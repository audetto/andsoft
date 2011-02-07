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
 castFloatE,
 castIntE,
 varFloatE,
 valueE,
 applyE,
 freeE,
 mkArray,
 readArr,
 (>*),
 (&&*)
)
where

  import Data.Ratio
  import Data.Set

  data DExp = LitFloat Float 
            | LitBool Bool
            | LitInt Int
            | Dummy                            -- used ONLY to as a placeholder to get the free variables
            | CastFloat DExp
            | CastInt DExp
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

  data Type = Bool | Float | Int
        deriving (Show, Eq)

  type Id = String

  type BoolE  = Exp Bool
  type FloatE = Exp Float
  type IntE   = Exp Int
  type ArrayE a = Exp [a]

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

  castFloatE :: IntE -> FloatE
  castFloatE (E x) = E (castFloatD x)

  castIntE :: FloatE -> IntE
  castIntE (E x) = E (castIntD x)

  notE  :: BoolE -> BoolE
  notE  = type1 notD

  (&&*) :: BoolE -> BoolE -> BoolE
  (&&*) = type2 andD

  freshVariable set name = let freshVar id = let try = name ++ (show id)
                                             in if member try set then freshVar (id + 1) else try
                           in freshVar 0

  mkArray :: IntE -> (IntE -> Exp a) -> ArrayE a
  mkArray (E size) item = let usedVars  = freeE (item (E Dummy))  -- apply item to dummy to get the free variables of item
                              id        = freshVariable usedVars "it"
                              dummy     = varIntE id
                              (E expr)  = item dummy
                          in E (MkArray id size expr)

  readArr :: ArrayE a -> IntE -> Exp a
  readArr = type2 readArrD

  freeE (E a) = freeD empty a

  intToFloat = fromInteger . toInteger

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

-- we should check the size
  readArrD (MkArray id _ expr) (LitInt a) = applyD [(id, intToFloat a)] expr
  readArrD arr (If c a b)           = ifD c (readArrD arr a) (readArrD arr b)
  readArrD arr pos                  = ReadArr arr pos

  castFloatD (LitInt a)             = LitFloat (intToFloat a)
  castFloatD (If c a b)             = ifD c (castFloatD a) (castFloatD b)
  castFloatD a                      = CastFloat a

  castIntD (LitFloat a)             = LitInt (truncate a)
  castIntD (If c a b)               = ifD c (castIntD a) (castIntD b)
  castIntD a                        = CastInt a


  -- simple direct valuation of the AST

  -- we only value FloatE!!!!!
  valueE :: [(Id, Float)] -> FloatE -> Float
  valueE ctx (E a) = valueFloat ctx a

  applyE ctx (E a) = E (applyD ctx a)

  valueBool  _ (LitBool a)         = a
  valueBool  ctx (If c a b)        = if (valueBool ctx c) then (valueBool ctx a) else (valueBool ctx b)
  valueBool  ctx (And a b)         = (valueBool ctx a) && (valueBool ctx b)
  valueBool  ctx (Not a)           = not (valueBool ctx a)
  valueBool  ctx (Positive a)      = (valueFloat ctx a) > 0.0

  valueInt _ (LitInt a)            = a
  valueInt ctx (CastInt a)         = truncate (valueFloat ctx a)
  valueInt ctx (Var a Int)         = let v = lookup a ctx
                                     in case v of Nothing -> error ("Missing " ++ a)
                                                  Just val -> truncate val

  valueFloat _ (LitFloat a)        = a
  valueFloat ctx (CastFloat a)     = intToFloat (valueInt ctx a)
  valueFloat ctx (Var a Float)     = let v = lookup a ctx
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
                                        newCtx x = (id, x) : ctx
                                        arr = Prelude.map (\x -> valueFloat (newCtx (intToFloat x)) e) [0 .. size - 1]
                                    in arr


-- apply the context to an expression

  applyD _ a@(LitFloat _)        = a
  applyD _ a@(LitBool _)         = a
  applyD _ a@(LitInt _)          = a
  applyD ctx a@(Var id Float)    = let v = lookup id ctx
                                   in case v of Nothing -> a
                                                Just val -> (LitFloat val)
  applyD ctx a@(Var id Int)      = let v = lookup id ctx
                                   in case v of Nothing -> a
                                                Just val -> (LitInt (truncate val))
  applyD ctx (If c a b)          = ifD (applyD ctx c) (applyD ctx a) (applyD ctx b)
  applyD ctx (Add a b)           = addD (applyD ctx a) (applyD ctx b)
  applyD ctx (Mul a b)           = mulD (applyD ctx a) (applyD ctx b)
  applyD ctx (Rec a)             = recD (applyD ctx a)
  applyD ctx (Negate a)          = negD (applyD ctx a)
  applyD ctx (Positive a)        = posD (applyD ctx a)
  applyD ctx (And a b)           = andD (applyD ctx a) (applyD ctx b)
  applyD ctx (Not a)             = notD (applyD ctx a)
  applyD ctx (Exponential a)     = notD (applyD ctx a)
  applyD ctx (Logarithm a)       = notD (applyD ctx a)
  applyD ctx (Sin a)             = notD (applyD ctx a)
  applyD ctx (Cos a)             = notD (applyD ctx a)
  applyD ctx (CastFloat a)       = castFloatD (applyD ctx a)
  applyD ctx (CastInt a)         = castIntD (applyD ctx a)
  applyD ctx (MkArray id a b)    = MkArray id (applyD ctx a) (applyD ctx b)
  applyD ctx (ReadArr a b)       = readArrD (applyD ctx a) (applyD ctx b)


-- free variables

  addFree fv e = foldl freeD fv e

  freeD fv (LitFloat _)        = fv
  freeD fv (LitBool _)         = fv
  freeD fv (LitInt _)          = fv
  freeD fv (Dummy)             = fv
  freeD fv (Var id _)          = insert id fv
  freeD fv (CastFloat a)       = addFree fv [a]
  freeD fv (CastInt a)         = addFree fv [a]
  freeD fv (If c a b)          = addFree fv [c, a, b]
  freeD fv (Add a b)           = addFree fv [a, b]
  freeD fv (Mul a b)           = addFree fv [a, b]
  freeD fv (Rec a)             = addFree fv [a]
  freeD fv (Negate a)          = addFree fv [a]
  freeD fv (Positive a)        = addFree fv [a]
  freeD fv (And a b)           = addFree fv [a, b]
  freeD fv (Not a)             = addFree fv [a]
  freeD fv (Exponential a)     = addFree fv [a]
  freeD fv (Logarithm a)       = addFree fv [a]
  freeD fv (Sin a)             = addFree fv [a]
  freeD fv (Cos a)             = addFree fv [a]
  freeD fv (ReadArr a b)       = addFree fv [a, b]
  freeD fv (MkArray id a b)    = union fv (delete id (addFree empty [a, b]))