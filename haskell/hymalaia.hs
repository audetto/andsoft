import Data.List

-- dates are just Int from 1 to N (the number of stocks)

hymalaia :: (Int -> String -> Double) -> [String] -> Double
hymalaia path names =
    let alive = names                   -- all stocks alive
        fixed = []                      -- no stock has fixed yet
        dates = [1 .. length names]     -- list of dates

        {-
          take a pair of names still alive and fixed values
          and a date
          and return a pair with one less alive and one more fixed value
         -}
        iteration :: ([String], [Double]) -> Int -> ([String], [Double])
        iteration (a, f) d = 
            let cmp s1 s2 = compare (path d s2) (path d s1) -- swap s1 and s2 to get the maximum
                sorted = sortBy cmp a
                best   = head sorted
            -- remove first and move its value to fixed
            -- because we return sorted and not a
            -- the order of the name will change but it does not matter
            -- I could remove the first occurence of best
            in (drop 1 sorted, (path d best):f)

        (_, f) = foldl iteration (alive, fixed) dates

    in sum f

path :: (Int -> String -> Double)
path d "S1" = [11,  8,  1] !! (d - 1)  -- "-1" since dates start at 1, but indexing at 0
path d "S2" = [10,  7, 13] !! (d - 1)
path d "S3" = [12, 12, 13] !! (d - 1)

-- we take 12 (S3) first time, 8 (S1) and then 13 (S2)

val = hymalaia path ["S1", "S2", "S3"]

-- it should be 33
