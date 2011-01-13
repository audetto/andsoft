module Elves.Date
(  
  Period(..)
, addPeriod
, createSchedule
)
where
      
import Data.Time.Calendar

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
