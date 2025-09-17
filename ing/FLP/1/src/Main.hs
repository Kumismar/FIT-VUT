-- Author: Ondřej Koumar (xkouma02), 2025
module Main where

import Inference (readAndClassify)
import System.Environment (getArgs)
import Training (trainTree)

main :: IO ()
main = do
    args <- getArgs
    case args of
        ("-1" : restArgs) -> readAndClassify restArgs
        ("-2" : restArgs) -> trainTree restArgs
        _ -> error $ "Invalid command-line args: " ++ show args
