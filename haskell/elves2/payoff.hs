import Data.Time.Calendar
import Data.List

data Period = Days Integer | Months Integer | Years Integer deriving (Show, Read, Eq)

-- addPeriod :: Period -> Day -> Day
addPeriod (Days   dd) d = addDays dd d
addPeriod (Months mm) d = addGregorianMonthsClip mm d
addPeriod (Years  yy) d = addGregorianYearsClip yy d

-- infinite schedule
createSchedule first inc =
    let s = first : map (addPeriod inc) s
    in s

type Schedule = [Day]

-- Value is a time series defined on the relevant schedule (getSchedule)
-- Here, Value is always single valued,
-- but in the target language it could be multivalued (e.g. to implement sort)
-- we need to introduce the "metadata" number of values
-- but then we need to check the size in Constructors that take single valued Value (e.g. Asian)

-- this is pure Data Representation, no code exists behind a Value
-- a Value is *never* executed in Haskell
-- only in the backends that support code execution
-- (e.g. Javascript, but not in LaTeX)

data Value = Number Schedule Double        -- primitive
           | Stock Schedule String         -- primitive
           | Asian Schedule Value          -- average with resets (should be derived and not here)
           | HorizOp String [Value]        -- this is like fold1 across values
           | VertOp String Value           -- this is like fold1 across times (e.g. "max(%d, %d)" or "%d + %d")
                                           --    where the string should be valid on each backend 
                                           --    (C++, Javascript, LaTeX...) (is it possible????)
                                           --    maybe we should use an enum of valid functions instead.
           | Resched Schedule Value        -- take the last value on or before the new schedule
           | Array Int [Value]             -- values[i]
           | Shift Value                   -- move values down 1 date (first date disappears)
           | Sort [Value]                  -- FIXME, it should return a multivalued Value (not supported yet)
             deriving (Show, Eq)

-- linearize the tree checking, skipping duplicate nodes
flatten root =
    let prependIfNotIn e l = if elem e l 
                             then l 
                             else e:l
        flat :: [Value] -> Value -> [Value]
        flat list n@(Number _ _)    = prependIfNotIn n list
        flat list s@(Stock  _ _)    = prependIfNotIn s list
        flat list a@(Asian  _ v)    = prependIfNotIn a (flat list v)
        flat list h@(HorizOp _ v)   = prependIfNotIn h (foldl flat list v)
        flat list t@(VertOp _ v)    = prependIfNotIn t (flat list v)
        flat list r@(Resched _ v)   = prependIfNotIn r (flat list v)
        flat list s@(Shift v)       = prependIfNotIn s (flat list v)
        flat list a@(Array _ v)     = prependIfNotIn a (foldl flat list v)
    in reverse (flat [] root)

-- get schedule on which this node is defined
getSchedule (Number s _)  = s
getSchedule (Stock s _)   = s
getSchedule (Asian s _)   = s
getSchedule (HorizOp _ v) = let n = nub (map getSchedule v)
                            in if length n == 1
                               then head n
                               else []
getSchedule (Array _ v)   = let n = nub (map getSchedule v)
                            in if length n == 1
                               then head n
                               else []
getSchedule (VertOp _ v)  = getSchedule v
getSchedule (Resched s _) = s
getSchedule (Shift v)     = drop 1 (getSchedule v)

-- check is not the same as "not null getSchedule" since Asian and Reschedule reset the schedule
-- we need to check all arguments
check (Number _ _)        = True
check (Stock _ _)         = True
check (Asian _ v)         = check v
check (VertOp _ v)        = check v
check h@(HorizOp _ _)     = not (null (getSchedule h)) -- they must be defined on the same schedule
check a@(Array _ _)       = not (null (getSchedule a)) -- they must be defined on the same schedule
check (Resched _ v)       = check v
check (Shift v)           = check v

-- merge all schedules of all nodes
allSchedule r = sort (nub (concat (map getSchedule (flatten r))))

-- helper to print linear list of values
showLinear f = concat (intersperse "\n" (map show f))

-- example
today = fromGregorian 2011 1 2

dates = take 3 (createSchedule today (Months 6))

expiry = [addPeriod (Months 3) (last dates)]

-- example of payoff (not even valid)
n1 = Number dates 4
n11 = VertOp "max" n1
n12 = Shift n11
n2 = Number dates 5
n3 = Number expiry 5
s = HorizOp "+" [n12, n2, n3]
p = Asian expiry s
a = Array 3 [n1,n2,n3,p]


r = a

f = flatten r

main = putStrLn (showLinear f)
