import System.Environment
import Text.Read
import Data.Maybe
import Control.Monad

safediv :: Int -> Int -> Maybe Int
safediv n m = if m == 0
  then Nothing
  else Just (div n m)

data Expr = Val Int | Div Expr Expr deriving(Show)

eval :: Expr -> Maybe Int
eval (Val n) = Just n
eval (Div x y) = eval x >>= (\ n ->
                  eval y >>= (\ m ->
                    safediv n m))

eval_do :: Expr -> Maybe Int
eval_do (Val n) = Just n
eval_do (Div x y) = do
  n <- eval x
  m <- eval y
  safediv n m

-- I HAVE A PLAN, no really.

readMaybeInt :: String -> Maybe Int
readMaybeInt s = readMaybe s

build_value :: String -> Maybe Expr
build_value s = fmap Val (readMaybeInt s)

build_div :: Expr -> Expr -> String -> Maybe Expr
build_div left right s = if s == "/"
  then Just $ Div left right else Nothing

fold_value :: [Expr] -> String -> Maybe [Expr]
fold_value stack input = (*>) mv $ Just $ (maybeToList mv) ++ stack
  where mv = build_value input

fold_div :: [Expr] -> String -> Maybe [Expr]
fold_div stack input = do
  right <- listToMaybe stack
  left <- listToMaybe (tail stack)
  let remaining = (tail.tail) stack
  let mv = build_div left right input
  (*>) mv $ Just $ (maybeToList mv) ++ remaining

build_somthing :: [Expr] -> String -> Maybe [Expr]
build_somthing stack input = (fold_value stack input) `mplus` (fold_div stack input)

fold_expr :: [Expr] -> [String] -> Maybe [Expr]
fold_expr stack inputs = foldM build_somthing stack inputs

maybeFromSingleList :: [a] -> Maybe a
maybeFromSingleList [x] = Just x
maybeFromSingleList (x : xs) = Nothing
maybeFromSingleList [] = Nothing

calc :: [String] -> Maybe Int
calc args = do
  values <- fold_expr [] args
  value <- maybeFromSingleList values
  result <- eval value
  return result

main = do
  args <- getArgs
  let maybe_result = calc args
  putStrLn $ maybe "ERROR" show maybe_result
