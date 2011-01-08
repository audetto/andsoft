import Data.Time.Calendar
import Data.List

data Period = Days Integer
            | Months Integer 
            | Years Integer 
              deriving (Show, Read, Eq)

-- addPeriod :: Period -> Day -> Day
addPeriod (Days   dd) d = addDays dd d
addPeriod (Months mm) d = addGregorianMonthsClip mm d
addPeriod (Years  yy) d = addGregorianYearsClip yy d

-- infinite schedule
createSchedule first inc =
    let s = first : map (addPeriod inc) s
    in s

type Schedule = [Day]

-- this is pure Data Representation, no code exists behind a Value
-- a Value is *never* executed in Haskell
-- only in the backends that support code execution
-- (e.g. Javascript, but not in LaTeX)

data Node = Val Value
          | Vec Vector
            deriving (Show, Eq)

data Value = Number Schedule Double        -- primitive
           | Stock Schedule String         -- primitive
           | Asian Schedule Value          -- average with resets (should be derived and not here)
           | HorizOp String [Value]        -- this is like fold1 across values
           | VertOp String Value           -- this is like fold1 across times (e.g. "max(%d, %d)" or "%d + %d")
                                           --    where the string should be valid on each backend 
                                           --    (C++, Javascript, LaTeX...) (is it possible????)
                                           --    maybe we should use an enum of valid functions instead.
           | Resched Schedule Value        -- take the last value on or before the new schedule
           | VecVal Int Vector             -- values[i]
           | Shift Value                   -- move values down 1 date (first date disappears)
             deriving (Show, Eq)

data Vector = Sort [Value]
            deriving (Show, Eq)

instance Num Value where
    (+) a b       = HorizOp "%d + %d" [a,b]
    (*) a b       = HorizOp "%d * %d" [a,b]
    (-) a b       = HorizOp "%d - %d" [a,b]
    negate a      = HorizOp "%d * %d" [a,Number [] (-1)]
    abs a         = HorizOp "abs(%d)" [a]
    signum a      = error "Not yet implemented"
    fromInteger x = Number [] (fromInteger x :: Double)

-- attempt to propagate schedules from one sibling to the next and down to the children
-- Asian, Resched & Shift cannot pass the schedule down since they introduce a new one
-- and they require their argument to have their own independent schedule
regenerateSchedules v = 
    let recreateFromSelf sched (Number [] d)   = Number sched d
        recreateFromSelf sched (VertOp str v)  = VertOp str (recreateFromSelf sched v)
        recreateFromSelf sched (HorizOp str v) = HorizOp str (map (recreateFromSelf sched) v)
        recreateFromSelf sched (Stock [] n)    = Stock sched n
        recreateFromSelf sched (VecVal i v)    = VecVal i (recreateFromSelf1 sched v)
        recreateFromSelf _     (Asian s v)     = Asian s (regenerateSchedules v)
        recreateFromSelf _     (Resched s v)   = Resched s (regenerateSchedules v)
        recreateFromSelf _     (Shift v)       = Shift (regenerateSchedules v)
        recreateFromSelf _ v                   = v
        recreateFromSelf1 sched (Sort v)       = Sort (map (recreateFromSelf sched) v)
    in recreateFromSelf (getSchedule (Val v)) v

-- direct children
getChildren (Val (Asian _ v))      = [Val v]
getChildren (Val (HorizOp _ v))    = map Val v
getChildren (Val (VertOp _ v))     = [Val v]
getChildren (Val (Resched _ v))    = [Val v]
getChildren (Val (Shift v))        = [Val v]
getChildren (Val (VecVal _ v))     = [Vec v]
getChildren (Vec (Sort v))         = map Val v
getChildren _                      = []

-- get the new schedule of this node
getNewSchedule (Val (Number s _))  = s
getNewSchedule (Val (Stock s _))   = s
getNewSchedule (Val (Asian s _))   = s
getNewSchedule (Val (Resched s _)) = s
getNewSchedule (Val (Shift v))     = drop 1 (getSchedule (Val v))
getNewSchedule _                   = []

-- get the schedule on which it is defined
getSchedule n = 
    let s = getNewSchedule n
    in if not (null s)
       then s
       else let as = nub (filter (not . null) (map getSchedule (getChildren n)))
            in case length as of 0 -> []
                                 1 -> head as
                                 _ -> error "Inconsistent schedules"

-- if every node has a schedule, then it is valid
check n = not (null (getSchedule n)) && all check (getChildren n)

-- linearize the tree skipping duplicate nodes
flatten root =
    let flat list n = if elem n list 
                      then list
                      else n : (foldl flat list (getChildren n))
    in reverse (flat [] root)

-- merge all schedules of all nodes
allSchedule r = sort (nub (concat (map getNewSchedule (flatten r))))

-- helper to print linear list of values
showLinear f = concat (intersperse "\n" (map show f))

-- example
today = fromGregorian 2011 1 2

dates = take 3 (createSchedule today (Months 6))

expiry = [addPeriod (Months 3) (last dates)]

-- example of payoff (not even valid)
n1 = Number [] 4
n11 = VertOp "max" n1
n12 = Shift n11
n2 = Number [] 5
n3 = Number [] 5
s = -(n1 + n2 + n3)
p = Asian expiry s
j = Number dates 6
jj = s + j
jjj = regenerateSchedules jj

a = Sort [n1, n2, n3]

r = (VecVal 1 a) + j

f = flatten (Val jjj)

main = putStrLn (showLinear f)
