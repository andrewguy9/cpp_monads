import System.Exit
import System.Environment
import Text.Read
import Data.Maybe
import Control.Monad

safeNeg :: Int -> Maybe Int
safeNeg x = Just $ -1 * x

safePlus :: Int -> Int -> Maybe Int
safePlus x y = Just $ x+y

safeSub :: Int -> Int -> Maybe Int
safeSub x y = Just $ x-y

safeMult :: Int -> Int -> Maybe Int
safeMult x y = Just $ x*y

safeDiv :: Int -> Int -> Maybe Int
safeDiv n m = if m == 0
  then Nothing
  else Just (div n m)

data Expr =
    Val Int         |
    Neg Expr        |
    Plus Expr Expr  |
    Sub Expr Expr   |
    Mult Expr Expr  |
    Div Expr Expr
  deriving(Show)

apply1 :: (Int -> Maybe Int) -> Expr -> Maybe Int
apply1 f x = do
  resultx <- eval x
  f resultx

apply2 :: (Int -> Int -> Maybe Int) -> Expr -> Expr -> Maybe Int
apply2 f x y = do
  resultX <- eval x
  resultY <- eval y
  f resultX resultY

eval :: Expr -> Maybe Int
eval (Val  n)   = Just n
eval (Neg  x  ) = apply1 safeNeg  x
eval (Plus x y) = apply2 safePlus x y
eval (Sub  x y) = apply2 safeSub  x y
eval (Mult x y) = apply2 safeMult x y
eval (Div  x y) = apply2 safeDiv  x y

readMaybeInt :: String -> Maybe Int
readMaybeInt s = readMaybe s

tryParseValue :: String -> Maybe Expr
tryParseValue s = fmap Val (readMaybeInt s)

tryParseStr1 :: (Expr -> Expr) -> String -> Expr -> String -> Maybe Expr
tryParseStr1 constructor opStr right token =
  if token == opStr then
    Just $ constructor right
  else
    Nothing

tryParseStr2 :: (Expr -> Expr -> Expr) -> String -> Expr -> Expr -> String -> Maybe Expr
tryParseStr2 constructor opStr left right token =
  if token == opStr then
    Just $ constructor left right
  else
    Nothing

tryParseNeg :: Expr -> String -> Maybe Expr
tryParseNeg right token = tryParseStr1 Neg "_" right token

tryParsePlus :: Expr -> Expr -> String -> Maybe Expr
tryParsePlus left right token = tryParseStr2 Plus "+" left right token

tryParseSub :: Expr -> Expr -> String -> Maybe Expr
tryParseSub left right token = tryParseStr2 Sub "-" left right token

tryParseMult :: Expr -> Expr -> String -> Maybe Expr
tryParseMult left right token = tryParseStr2 Mult "*" left right token

tryParseDiv :: Expr -> Expr -> String -> Maybe Expr
tryParseDiv left right token = tryParseStr2 Div "/" left right token

fold0 :: (String -> Maybe Expr) -> [Expr] -> String -> Maybe [Expr]
fold0 tryParse stack token = do
  v <- tryParse token
  return $ v : stack

fold1 :: (Expr -> String -> Maybe Expr) -> [Expr] -> String -> Maybe [Expr]
fold1 tryParse stack token = do
  right <- listToMaybe stack
  let remaining = tail stack
  v <- tryParse right token
  return $ v : remaining

fold2 :: (Expr -> Expr -> String -> Maybe Expr) -> [Expr] -> String -> Maybe [Expr]
fold2 tryParse stack token = do
  right <- listToMaybe stack
  left <- listToMaybe (tail stack)
  let remaining = (tail.tail) stack
  v <- tryParse left right token
  return $ v : remaining

fold_value :: [Expr] -> String -> Maybe [Expr]
fold_value stack token = fold0 tryParseValue stack token

fold_neg :: [Expr] -> String -> Maybe [Expr]
fold_neg stack token = fold1 tryParseNeg stack token

fold_plus :: [Expr] -> String -> Maybe [Expr]
fold_plus stack token = fold2 tryParsePlus stack token

fold_sub :: [Expr] -> String -> Maybe [Expr]
fold_sub stack token = fold2 tryParseSub stack token

fold_mult :: [Expr] -> String -> Maybe [Expr]
fold_mult stack token = fold2 tryParseMult stack token

fold_div :: [Expr] -> String -> Maybe [Expr]
fold_div stack token = fold2 tryParseDiv stack token

build_somthing :: [Expr] -> String -> Maybe [Expr]
build_somthing stack input =
  (fold_value stack input)
  `mplus`
  (fold_neg stack input)
  `mplus`
  (fold_plus stack input)
  `mplus`
  (fold_sub stack input)
  `mplus`
  (fold_mult stack input)
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

run :: [String] -> Maybe Int
run args = do
  expr <- parse args
  result <- eval expr
  return result

main = do
  args <- getArgs
  let maybe_result = run args
  putStrLn $ maybe "ERROR" show maybe_result
  if isJust maybe_result then
    exitFailure
  else
    exitSuccess
