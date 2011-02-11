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
 forcedE,
 mkArray,
 readArr,
 (>*),
 (&&*)
)
where

  import Elves.Exp
  import Elves.Value
