{-# LANGUAGE TypeSynonymInstances #-}
module Elves.Value
where

  import Elves.Exp
  import Data.Map

  -- simple direct valuation of the AST

  -- we only value FloatE!!!!!
  valueE :: Map Id Float -> FloatE -> Float
  valueE ctx (E a) = valueFloat ctx a

  valueBool  _ (LitBool a)         = a
  valueBool  ctx (If c a b)        = if (valueBool ctx c) then (valueBool ctx a) else (valueBool ctx b)
  valueBool  ctx (And a b)         = (valueBool ctx a) && (valueBool ctx b)
  valueBool  ctx (Not a)           = not (valueBool ctx a)
  valueBool  ctx (Positive a)      = (valueFloat ctx a) > 0.0

  valueInt _ (LitInt a)            = a
  valueInt ctx (CastInt a)         = truncate (valueFloat ctx a)
  valueInt ctx (Var a Int)         = let v = Data.Map.lookup a ctx
                                     in case v of Nothing -> error ("Missing " ++ a)
                                                  Just val -> truncate val

  valueFloat _ (LitFloat a)        = a
  valueFloat ctx (CastFloat a)     = intToFloat (valueInt ctx a)
  valueFloat ctx (Var a Float)     = let v = Data.Map.lookup a ctx
                                     in case v of Nothing -> error ("Missing " ++ a)
                                                  Just val -> val
  valueFloat ctx (If c a b)        = if (valueBool ctx c) then (valueFloat ctx a) else (valueFloat ctx b)
  valueFloat ctx (Add a b)         = (valueFloat ctx a) + (valueFloat ctx b)
  valueFloat ctx (Mul a b)         = (valueFloat ctx a) * (valueFloat ctx b)
  valueFloat ctx (Rec a)           = 1 / (valueFloat ctx a)
  valueFloat ctx (Negate a)        = 0 - (valueFloat ctx a)
  valueFloat ctx (Exponential a)   = exp (valueFloat ctx a)
  valueFloat ctx (Logarithm a)     = log (valueFloat ctx a)
  valueFloat ctx (Sin a)           = sin (valueFloat ctx a)
  valueFloat ctx (Cos a)           = cos (valueFloat ctx a)
  valueFloat ctx (ReadArr arr pos) = let a = valueArray ctx arr
                                         p = valueInt   ctx pos
                                     in a !! p

  valueArray ctx (MkArray id s e) = let size = valueInt ctx s
                                        newCtx x = Data.Map.insert id x ctx
                                        arr = Prelude.map (\x -> valueFloat (newCtx (intToFloat x)) e) [0 .. size - 1]
                                    in arr
