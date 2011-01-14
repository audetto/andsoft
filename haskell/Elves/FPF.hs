module Elves.FPF
(
  FPFVal
, FPFDouble
, FPFAsset
, FPFList
, Elves.FPF.min
, Elves.FPF.max
, pow
, Elves.FPF.log
, Elves.FPF.exp
, cond
, name
, observe
)
where

import Elves.Date
import Data.Time.Calendar

type FPFAsset = String

data FPFDouble = Add FPFDouble FPFDouble
               | Sub FPFDouble FPFDouble
               | Mul FPFDouble FPFDouble
               | Div FPFDouble FPFDouble
               | Min FPFDouble FPFDouble
               | Max FPFDouble FPFDouble
               | Pow FPFDouble FPFDouble
               | Abs FPFDouble
               | Log FPFDouble
               | Exp FPFDouble
               | Const Double 
               | Cond FPFBool FPFDouble FPFDouble
               | Asset FPFAsset Day
                 deriving (Show, Eq)

data FPFBool = C Bool
             | Not FPFBool
             | And FPFBool FPFBool
             | Or FPFBool FPFBool
             | Less FPFDouble FPFDouble
               deriving (Show, Eq)

data FPFVal = Dbl FPFDouble
            | Bl  FPFBool
              deriving (Show, Eq)

type FPFList a = [a]

instance Num FPFDouble where
    (+) a b       = Add a b
    (*) a b       = Mul a b
    (-) a b       = Sub a b
    negate a      = 0 - a
    abs a         = Abs a
    signum a      = error "Not yet implemented"
    fromInteger x = Const (fromInteger x :: Double)

instance Fractional FPFDouble where
  (/) a b = Div a b
  recip a = 1 / a
  fromRational x = Const (fromRational x :: Double)

class FPFOrd a where
    (<), (<=), (>=), (>) :: a -> a -> FPFBool
    max, min             :: a -> a -> a
    (>=) a b       = Not (a Elves.FPF.< b)
    (<=) a b       = Not (a Elves.FPF.> b)
    (>)  a b       = error "Who knows?"
                    
instance FPFOrd FPFDouble where
    (<) a b        = Less a b
    max  a b       = Max a b
    min  a b       = Min a b

pow a b = Pow a b
log a   = Log a
exp a   = Exp a

cond a b c = Cond a b c

observe a d = Asset a d

name s a = a
