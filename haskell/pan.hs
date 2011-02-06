import Elves.Pan

one = 1 :: FloatE
x = varFloatE "x"
ctx x = if x == "x" then 5 else error x

f x = x * x
arr = mkArray 5 f
pos = ifE (x >* 5) (1 :: IntE) 2
ele = readArr arr pos
