import System.Exit
import System.Environment
import Text.Read
import Data.Maybe
import Control.Monad

safediv :: Int -> Int -> Maybe Int
safediv n m = if m == 0
  then Nothing
  else Just (div n m)

--TODO Add other operators
data Expr = Val Int | Div Expr Expr deriving(Show)

eval :: Expr -> Maybe Int
eval (Val n) = Just n
--TODO use `do`
eval (Div x y) = eval x >>= (\ n ->
                  eval y >>= (\ m ->
                    safediv n m))

readMaybeInt :: String -> Maybe Int
readMaybeInt s = readMaybe s

tryParseValue :: String -> Maybe Expr
tryParseValue s = fmap Val (readMaybeInt s)

tryParseDiv :: Expr -> Expr -> String -> Maybe Expr
tryParseDiv left right s = if s == "/"
  then Just $ Div left right else Nothing

fold_value :: [Expr] -> String -> Maybe [Expr]
fold_value stack input = do
  v <- tryParseValue input
  return $ v : stack

fold_div :: [Expr] -> String -> Maybe [Expr]
fold_div stack input = do
  right <- listToMaybe stack
  left <- listToMaybe (tail stack)
  let remaining = (tail.tail) stack
  v <- tryParseDiv left right input
  return $ v : remaining

build_somthing :: [Expr] -> String -> Maybe [Expr]
build_somthing stack input =
  (fold_value stack input)
  `mplus`
  (fold_div stack input)

maybeFromSingleList :: [a] -> Maybe a
maybeFromSingleList [x] = Just x
maybeFromSingleList (x : xs) = Nothing
maybeFromSingleList [] = Nothing

parse :: [String] -> Maybe Expr
parse tokens = do
  exprStack <- foldM build_somthing [] tokens
  maybeFromSingleList exprStack

calc :: [String] -> Maybe Int
calc args = do
  expr <- parse args
  result <- eval expr
  return result

main = do
  args <- getArgs
  let maybe_result = calc args
  putStrLn $ maybe "ERROR" show maybe_result
  if isJust maybe_result then
    exitFailure
  else
    exitSuccess
