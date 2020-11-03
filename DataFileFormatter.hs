module Main where

-- tested with GHCi 8.8.3

import qualified System.Environment as Sys

main :: IO ()
main = Sys.getArgs >>= dispatch

dispatch :: [String] -> IO ()
dispatch [] = do 
    progName <- Sys.getProgName
    print $ "Usage: " ++ progName ++ " *.dat"

dispatch args = mapM_ (\a -> print a >> processFile a) args

processFile :: String -> IO ()
processFile fileName = 
    reformatDataList . concatMap words . lines 
    <$> readFile fileName 
    >>= writeFile (fileName ++ "2")

reformatDataList ::[String] -> String
reformatDataList [] = error "EMPTY FILE"
reformatDataList (matrixSize:data_) = 
    let matrixSizeInt      = fromIntegral (read matrixSize)
        expectedWordsCount = matrixSizeInt ^ 2 * 2
        actualWordsCount   = length data_
    in if actualWordsCount /= expectedWordsCount
        then error $ "WRONG WORDS COUNT: EXPECTED " ++ show expectedWordsCount ++ ", GOT " ++ show actualWordsCount
        else formatFile matrixSizeInt . makeLines matrixSizeInt $ data_
                
makeLines :: Int -> [String] -> [[String]]
makeLines _ [] = []
makeLines count data_ = take count data_ : makeLines count (drop count data_)

formatFile :: Int -> [[String]] -> String
formatFile matrixSize data_ = 
    let linesInMatrix = matrixSize
        fstPart = fmap unwords (take linesInMatrix data_)
        sndPart = fmap unwords (drop linesInMatrix data_)
    in  unlines $ show matrixSize : "" : fstPart ++ [""] ++ sndPart
