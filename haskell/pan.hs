import Elves.Pan

one = 1 :: FloatE
x = varFloatE "x"
ctx = [("x", 4 :: Float)]

f y = (castFloatE y) * x
arr = mkArray 5 f
pos = ifE (x >* 5) (1 :: IntE) 2
ele = readArr arr (castIntE x)

chk = ifE (x >* 5) one (one + one)
