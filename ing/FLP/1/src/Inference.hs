-- Module: Inference
-- Author: Ondřej Koumar (xkouma02), 2025
module Inference (readAndClassify) where

import Data.List.Split (splitOn)

data DecisionTree
    = Leaf String
    | Node Int Float DecisionTree DecisionTree
    deriving (Show)

-- Represents a single line parsed from the decision tree definition file.
data ParsedLine -- Node: indentation, feature index, threshold.
    = NodeLine Int Int Float
    | -- Leaf: indentation, class name.
      LeafLine Int String

{- | Take one line from the input tree file, trim it and transform it
  into an equivalent ParsedLine.
-}
parseLine :: String -> ParsedLine
parseLine line =
    let indent = length $ takeWhile (== ' ') line
        trimmedLine = dropWhile (== ' ') line
     in case words trimmedLine of
            ("Node:" : featureStr : thresholdStr : _) ->
                NodeLine indent (read $ init featureStr) (read thresholdStr)
            ("Leaf:" : classStr : _) -> LeafLine indent classStr
            _ -> error $ "Invalid line format: " ++ line ++ " bruh: " ++ show (words trimmedLine)

{- | An abstraction to buildTree' function, which does most of the tree construction.
  Reason for having this function is to return the final DecisionTree only, while
  buildTree' is passing around the unprocessed part of the input file.
-}
buildTree :: [ParsedLine] -> DecisionTree
buildTree [] = Leaf "sample"
buildTree linesFromFile = fst $ buildTree' linesFromFile

{- | Core of the tree construction. The tree itself is created by "preorder traversal" of
  pseudotree saved as ParsedLines in a list. The algorithm recursively creates left
  subtrees until a leaf is hit; then, right subtree is constructed and finally their
  parent. Indent increments by two with each recursion call. Each subtree returns what
  is yet left to construct.
-}
buildTree' :: [ParsedLine] -> (DecisionTree, [ParsedLine])
buildTree' ((LeafLine _ className) : restLines) = (Leaf className, restLines)
buildTree' ((NodeLine _ index threshold) : restLines) =
    let (leftTree, restAfterLeft) = buildTree' restLines
        -- We need to construct the right subtree with what is left after constructing the left subtree.
        (rightTree, restAfterRight) = buildTree' restAfterLeft
     in (Node index threshold leftTree rightTree, restAfterRight)
buildTree' _ = error "How did we get here? buildTree'"

-- | Divide the input tree file into lines and process each one.
processFile :: String -> [ParsedLine]
processFile = map parseLine . lines

{- | An encapsulation to all the important functions working with the decision tree. Reads
  the text from tree file, parses it and executes the classification process on input data.
-}
readAndClassify :: [String] -> IO ()
readAndClassify [treeFile, dataFile] = do
    treeFileContents <- readFile treeFile
    let parsedLines = processFile treeFileContents
    let tree = buildTree parsedLines
    dataFileContents <- readFile dataFile
    let dataPoints = map (map read . splitOn ",") $ lines dataFileContents
    let predictions = map (classify tree) dataPoints
    mapM_ putStrLn predictions
readAndClassify _ = error "Expected two arguments, tree file and data file."

-- | Traverses the classification tree and returns the class which data point belongs to.
classify :: DecisionTree -> [Float] -> String
classify (Leaf className) _ = className
classify (Node feature threshold left right) dataPoint
    | dataPoint !! feature <= threshold = classify left dataPoint
    | otherwise = classify right dataPoint
