{-# LANGUAGE TypeSynonymInstances #-}
module Elves.Pan
(
 Id,
 FloatE,
 BoolE,
 IntE,
 litBoolE,
 ifE,
 notE,
 varFloatE,
 value,
 mkArray,
 readArr,
 (>*),
 (&&*)
)
where

  import Data.Ratio

  data DExp = LitFloat Float 
            | LitBool Bool
            | LitInt Int
            | Var Id Type
            | If DExp DExp DExp
            | Add DExp DExp
            | Mul DExp DExp
            | Rec DExp
            | Negate DExp
            | Positive DExp
            | And DExp DExp
            | Not DExp
            | Exponential DExp
            | Logarithm DExp
            | Sin DExp
            | Cos DExp
            | MkArray Id DExp DExp        -- dummy size expr
            | ReadArr DExp DExp           -- array pos
              deriving (Show, Eq)

  data Exp a = E DExp
        deriving (Show, Eq)

  data ArrayE a = EE DExp
        deriving (Show, Eq)
     
  data Type = Bool | Float | Int
        deriving (Show, Eq)

  type Id = String

  type BoolE  = Exp Bool
  type FloatE = Exp Float
  type IntE   = Exp Int

  get (E a) = a
  gett (EE a) = a

  type1 :: (DExp -> DExp) -> (Exp a -> Exp b)
  type2 :: (DExp -> DExp -> DExp) -> (Exp a -> Exp b -> Exp c)
  type3 :: (DExp -> DExp -> DExp -> DExp) -> (Exp a -> Exp b -> Exp c -> Exp d)

  type1 f (E a) = E (f a)
  type2 f (E a) (E b) = E (f a b)
  type3 f (E a) (E b) (E c) = E (f a b c)

  instance Num FloatE where
      (+)                           = type2 addD
      (*)                           = type2 mulD
      (-) a b                       = a + (negate b)
      negate                        = type1 negD
      abs a                         = ifE (a >* 0.0) a (negate a)
      signum                        = type1 signumD
      fromInteger                   = E . LitFloat . fromInteger

  instance Num IntE where
      (+)                           = error "Not Yet"
      (*)                           = type2 mulD
      (-) a b                       = error "Not Yet"
      negate                        = error "Not Yet"
      abs a                         = error "Not Yet"
      signum                        = error "Not Yet"
      fromInteger                   = E . LitInt . fromInteger

  instance Fractional FloatE where
      (/) a b                       = a * (recip b)
      recip                         = type1 recD
      fromRational a                = E (LitFloat ((fromInteger (numerator a)) / (fromInteger (denominator a))))

  instance Floating FloatE where
      exp                           = type1 expD
      log                           = type1 logD
      sin                           = type1 sinD
      cos                           = type1 cosD
      pi                            = error "Not Yet"
      asin                          = error "Not Yet"
      atan                          = error "Not Yet"
      acos                          = error "Not Yet"
      sinh                          = error "Not Yet"
      cosh                          = error "Not Yet"
      asinh                         = error "Not Yet"
      acosh                         = error "Not Yet"
      atanh                         = error "Not Yet"

  instance Ord FloatE where
      compare                       = error "Cannot"
      min a b                       = ifE (a >* b) b a
      max a b                       = ifE (a >* b) a b

-- E funcions

  class Synctactic a where
       ifE   :: BoolE -> a -> a -> a
       (>*)  :: a -> a -> BoolE

  instance Synctactic (Exp a) where
       ifE   = type3 ifD
       (>*)  = type2 gD

  litBoolE :: Bool -> BoolE
  litBoolE = E . LitBool

  varFloatE :: String -> FloatE
  varFloatE n = E (Var n Float)

  varIntE :: String -> IntE
  varIntE n = E (Var n Int)

  notE  :: BoolE -> BoolE
  notE  = type1 notD

  (&&*) :: BoolE -> BoolE -> BoolE
  (&&*) = type2 andD

  mkArray :: IntE -> (IntE -> Exp a) -> ArrayE a
  mkArray size func = let id = "dummy"
                          dummy = varIntE id
                          expr  = func dummy
                      in EE (MkArray id (get size) (get expr))

  readArr :: ArrayE a -> IntE -> Exp a
  readArr arr pos = E (ReadArr (gett arr) (get pos))

-- D functions

  recD (LitFloat a)                 = LitFloat (1 / a)
  recD (Rec a)                      = a
  recD (If c a b)                   = ifD c (recD a) (recD b)
  recD a                            = Rec a

  addD (LitFloat a) (LitFloat b)    = LitFloat (a + b)
  addD (LitFloat 0.0) b             = b
  addD a (LitFloat 0.0)             = a
  addD (If c a b) d                 = ifD c (addD a d) (addD b d)
  addD d (If c a b)                 = ifD c (addD d a) (addD d b)
  addD a b
      | a == negD b                 = LitFloat 0.0
  addD a b                          = Add a b

  mulD (LitFloat a) (LitFloat b)    = LitFloat (a * b)
  mulD (LitFloat 0.0) _             = LitFloat 0.0
  mulD _ (LitFloat 0.0)             = LitFloat 0.0
  mulD (LitFloat 1.0) b             = b
  mulD b (LitFloat 1.0)             = b
  mulD (If c a b) d                 = ifD c (mulD a d) (mulD b d)
  mulD d (If c a b)                 = ifD c (mulD d a) (mulD d b)
  mulD a b
      | a == recD b                 = LitFloat 1.0
  mulD a b = Mul a b

  negD (LitFloat a)                 = LitFloat (0.0 - a)
  negD (Negate a)                   = a
  negD (If c a b)                   = ifD c (negD a) (negD b)
  negD a                            = Negate a

  signumD (LitFloat a)              = LitFloat (signum a)
  signumD (If c a b)                = ifD c (signumD a) (signumD b)
  signumD a = a

  ifD (LitBool True) a _            = a 
  ifD (LitBool False) _ b           = b
  ifD (Not c) a b                   = ifD c b a
  ifD (If c d e) a b                = ifD c (ifD d a b) (ifD e a b)
  ifD c a b
      | a == b                      = a
  ifD c a b                         = If c a b

  posD (LitFloat a)                 = LitBool (a > 0.0)
  posD (If c a b)                   = ifD c (posD a) (posD b)
  posD a                            = Positive a

  gD a b                            = posD (addD a (negD b))

  notD (LitBool True)               = LitBool False
  notD (LitBool False)              = LitBool True
  notD (Not a)                      = a
  notD a                            = Not a

  andD (LitBool a) (LitBool b)      = LitBool (a && b)
  andD _ (LitBool False)            = LitBool False
  andD (LitBool False) _            = LitBool False
  andD a (LitBool True)             = a
  andD (LitBool True) b             = b
  andD (If c a b) d                 = ifD c (andD a d) (andD b d)
  andD d (If c a b)                 = ifD c (andD d a) (andD d b)
  andD a b
      | a == b                      = a
      | a == notD b                 = LitBool False
  andD a b                          = And a b

  sinD (LitFloat a)                 = LitFloat (sin a)
  sinD (If c a b)                   = ifD c (sinD a) (sinD b)
  sinD a                            = Sin a 

  cosD (LitFloat a)                 = LitFloat (cos a)
  cosD (If c a b)                   = ifD c (cosD a) (cosD b)
  cosD a                            = Cos a 

  expD (LitFloat a)                 = LitFloat (exp a)
  expD (If c a b)                   = ifD c (expD a) (expD b)
  expD (Logarithm a)                = a
  expD a                            = Exponential a 

  logD (LitFloat a)                 = LitFloat (log a)
  logD (If c a b)                   = ifD c (logD a) (logD b)
  logD (Exponential a)              = a
  logD a                            = Logarithm a 

  -- simple direct valuation of the AST

  value ctx a = valueFloat ctx (get a)

  valueBool  _ (LitBool a)         = a
  valueBool  ctx (If c a b)        = if (valueBool ctx c) then (valueBool ctx a) else (valueBool ctx b)
  valueBool  ctx (And a b)         = (valueBool ctx a) && (valueBool ctx b)
  valueBool  ctx (Not a)           = not (valueBool ctx a)
  valueBool  ctx (Positive a)      = (valueFloat ctx a) > 0.0

  valueInt _ (LitInt a)            = a
  valueInt ctx (Var a Int)         = truncate (ctx a)

  valueFloat _ (LitFloat a)        = a
  valueFloat _ (LitInt a)          = fromInteger (toInteger a)
  valueFloat ctx (Var a Float)     = ctx a
  valueFloat ctx (Var a Int)       = ctx a
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
  valueFloat ctx x                 = error (show x)

  valueArray ctx (MkArray id s e) = let size = valueFloat ctx s
                                        newCtx val name = if name == id then val else ctx name
                                        arr = map (\x -> valueFloat (newCtx x) e) [0 .. size - 1]
                                    in arr
