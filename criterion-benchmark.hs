{-# INCLUDE "benchmark.h" #-}
{-# LANGUAGE ForeignFunctionInterface #-}
module Main where
import Foreign.C -- get the C types
import Criterion.Main
 
-- "unsafe" means it's slightly faster but can't callback to haskell
foreign import ccall unsafe "benchmark_parseURL" benchmark :: CInt -> CInt
 
main = defaultMain [
          bgroup "parseURL" [bench title $ whnf (benchmark) n]
          ]
  where
    n = 1000000
    title = "benchmark " ++ (show n) 

