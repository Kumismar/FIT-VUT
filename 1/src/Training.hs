-- Module: Training
-- Author: Ondřej Koumar (xkouma02), 2025

module Training (trainTree, removeCommas) where

import Data.List (group, maximumBy, minimumBy, partition, sort, transpose)
import Data.Ord (comparing)

type FeatureValue = Double
type ClassLabel = String
type FeatureIndex = Int
type WeightedGini = Double
type Threshold = Double
type GiniIndex = Double

-- Data structure representing one input row from a file,
-- e.g. a number of features and class name at the end of the row.
type Record = ([FeatureValue], ClassLabel)

-- Data structure representing the decision tree.
data DecisionTree
    = Leaf ClassLabel
    | Node
        { attributeIndex :: Int
        , threshold :: FeatureValue
        , left :: DecisionTree
        , right :: DecisionTree
        }
    deriving (Show, Eq)

-- | Substitute comma with a space character.
removeCommas :: Char -> Char
removeCommas c
    | c == ',' = ' '
    | otherwise = c

{- | Remove commas from lines and substitute them with whitespaces. Then, divide each
  line into a list of words. Each words represents a feature value or specific class.
-}
processFile :: String -> [[String]]
processFile = map (words . map removeCommas) . lines

-- | Take one row from the input file and create Record data structure from it.
parseRow :: [String] -> Record
parseRow row = (features, className)
  where
    className = last row
    featuresStr = init row
    features = map read featuresStr

-- | Print 'number' spaces to stdout. Used for printing indentation in the decision tree.
printIndent :: Int -> IO ()
printIndent number = putStr $ replicate number ' '

-- | Traverse the tree in an inorder manner and print all the nodes to stdout.
writeTree :: DecisionTree -> Int -> IO ()
writeTree (Leaf label) indent = do
    printIndent indent
    putStrLn $ "Leaf: " ++ show label
writeTree (Node index threshold left right) indent = do
    printIndent indent
    putStrLn $ "Node: " ++ show index ++ ", " ++ show threshold
    writeTree left (indent + 2)
    writeTree right (indent + 2)

-- | Read input file contents and build a decision tree.
readAndTrain :: FilePath -> IO ()
readAndTrain fileName = do
    fileContent <- readFile fileName
    let dataSet = map parseRow $ processFile fileContent
    let tree = trainTree' dataSet
    writeTree tree 0

{- | The only function encapsulating the whole tree training process. Only checks
  if the command-line arguments are correctly given to the program.
-}
trainTree :: [String] -> IO ()
trainTree [fileName] = readAndTrain fileName
trainTree _ = error "Usage for desicion tree training: ./flp-fun -2 filePath"

{- | Construct the decision tree recursively.
  Return a Leaf node if the dataset at the current node contains samples of only one class.
  Otherwise, identify the optimal feature and threshold, partition the dataset
  into left and right subsets according to this split criterion, and then recursively
  build the corresponding left and right child subtrees.
-}
trainTree' :: [Record] -> DecisionTree
trainTree' dataSet
    | null dataSet = error "empty dataset"
    | allTheSameClass dataSet = Leaf $ snd $ head dataSet
    | otherwise =
        if null leftData || null rightData
            then
                -- Based on what I've read on https://scikit-learn.org/stable/modules/tree.html, section 1.10.5,
                -- when not enough samples are either in left or right sub-dataset, a leaf is created with the
                -- majority class. By default, in Python, at least 2 samples have to be in each sub-dataset; here,
                -- I chose just one, even though the tree is prone to overfitting.
                Leaf majClass
            else
                Node
                    bestIndex
                    bestThreshold
                    (trainTree' leftData)
                    (trainTree' rightData)
  where
    (bestIndex, bestThreshold) = findBestSplit dataSet
    (leftData, rightData) = partition (\(features, _) -> (features !! bestIndex) < bestThreshold) dataSet
    majClass = calculateMajorityClass dataSet

-- | Take a dataset and compute the most common class in it.
calculateMajorityClass :: [Record] -> ClassLabel
calculateMajorityClass records
    | null records = error "Cannot calculate majority class of an empty dataset"
    | otherwise =
        let labels = map snd records
            sortedLabels = sort labels
            groupedLabels = group sortedLabels
            majorityGroup = maximumBy (comparing length) groupedLabels
         in head majorityGroup

-- | Find the best feature/attribute and its threshold for creating a tree node.
findBestSplit :: [Record] -> (FeatureIndex, FeatureValue)
findBestSplit dataset =
    let features = transpose $ map fst dataset
        labels = map snd dataset
        indexedFeatures = zip [0 ..] features
     in selectBestSplit $ map (\(index, values) -> findBestSplitForFeature index values labels) indexedFeatures

-- | For given attribute/feature, compute potential thresholds and select the best one.
findBestSplitForFeature :: FeatureIndex -> [FeatureValue] -> [ClassLabel] -> (FeatureIndex, FeatureValue, WeightedGini)
findBestSplitForFeature index featureValues labels = (index, threshold, gini)
  where
    potentialThresholds = calculatePotentialThresholds $ sort featureValues
    (threshold, gini) = selectBestThreshold potentialThresholds featureValues labels

{- | Create tuples with thresholds and corresponding left and right sub-dataset with
  values < or >= than threshold, respectively. Then, for each tuple, calculate weighted
  gini index and select a threshold with the lowest weighted gini.
-}
selectBestThreshold :: [Threshold] -> [FeatureValue] -> [ClassLabel] -> (Threshold, WeightedGini)
selectBestThreshold thresholds featureValues labels =
    let valuesAndLabels = zip featureValues labels
        thresholdsAndDividedValues :: [(Threshold, ([(FeatureValue, ClassLabel)], [(FeatureValue, ClassLabel)]))]
        thresholdsAndDividedValues = map (\threshold -> (threshold, partition (\(featureValue, _) -> featureValue < threshold) valuesAndLabels)) thresholds
        thresholdsAndGinis :: [(Threshold, WeightedGini)]
        thresholdsAndGinis = map calculateWeightedGini thresholdsAndDividedValues
     in minimumBy (comparing snd) thresholdsAndGinis

{- | Calculate the weighted Gini index for a given threshold and data partitioned into two lists.
  Left list contains values < threshold, right list contains values >= threshold.
-}
calculateWeightedGini :: (Threshold, ([(FeatureValue, ClassLabel)], [(FeatureValue, ClassLabel)])) -> (Threshold, WeightedGini)
calculateWeightedGini (threshold, (left, right)) = (threshold, wGini)
  where
    lenLD = fromIntegral (length left) :: Double
    lenRD = fromIntegral (length right) :: Double
    lenData = lenLD + lenRD
    wGini = (lenLD / lenData) * calculateGiniIndex left + (lenRD / lenData) * calculateGiniIndex right

{- | Compute gini index from given dataset.
  1) Calculate number of occurences of each class in the dataset.
  2) Calculate length of the dataset.
  3) For each class, compute probability of the class in the dataset -> prob = N_i/N.
  4) Return 1 - \sum{i \in C}{P_i^2}, where C is a set of classes in the dataset.
-}
calculateGiniIndex :: [(FeatureValue, ClassLabel)] -> GiniIndex
calculateGiniIndex dataset =
    let labels = map snd dataset
        calculateCounts = map length . group . sort
        counts = map fromIntegral $ calculateCounts labels
        dataLen = fromIntegral (length dataset) :: Double
        classProbabilities = map (/ dataLen) counts
        cpSquared = map (\prob -> prob * prob) classProbabilities
     in 1.0 - sum cpSquared

{- | Takes values of one attribute and calculates potential thresholds.
  For each pair of values computes an arithmetic mean.
-}
calculatePotentialThresholds :: [FeatureValue] -> [Threshold]
calculatePotentialThresholds sortedValues = zipWith (\a b -> (a + b) / 2) sortedValues (tail sortedValues)

{- | Takes a list of calculated weighted gini values for each instance
  of data attributes and selects the lowest one.
-}
selectBestSplit :: [(FeatureIndex, FeatureValue, WeightedGini)] -> (FeatureIndex, FeatureValue)
selectBestSplit bestAttributeSplits = (bestIndex, bestThreshold)
  where
    (bestIndex, bestThreshold, _) = minimumBy (comparing (\(_, _, gini) -> gini)) bestAttributeSplits

-- | Checks if all the data belong to the same class.
allTheSameClass :: [Record] -> Bool
allTheSameClass [_] = True
allTheSameClass (firstRecord : rest) = all ((== firstClassName) . snd) rest
  where
    firstClassName = snd firstRecord
allTheSameClass _ = error "How did we get here? allTheSameClass"
