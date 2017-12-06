-- Note this program is written in a C++ compatible style to aid in porting it to C++.
-- It does not exemplify good Haskell style.

-- Note: Avoid things which are hard to port to C++.
-- * Pattern Matching
-- * Do syntax
-- * Partial function application

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

exprFold0 :: (String -> Maybe Expr) -> [Expr] -> String -> Maybe [Expr]
exprFold0 tryParse stack token =
  ms1 >>=
    (\ x -> Just $ x:stack)
  where ms1 = (tryParse token)

exprFold1 :: (Expr -> String -> Maybe Expr) -> [Expr] -> String -> Maybe [Expr]
exprFold1 tryParse stack token =
  (listToMaybe stack) >>=
    (\s1 -> (tryParse s1 token) >>= (\ x -> Just $ x:remaining))
  where remaining = tail stack

exprFold2 :: (Expr -> Expr -> String -> Maybe Expr) -> [Expr] -> String -> Maybe [Expr]
exprFold2 tryParse stack token =
  ms1 >>=
    (\ s1 -> ms2 >>= (\ s2 -> tryParse s2 s1 token)) >>=
    (\ x -> Just $ x:remaining)
  where ms1 = listToMaybe stack
        ms2 = listToMaybe (tail stack)
        remaining = (tail.tail) stack

fold_value :: [Expr] -> String -> Maybe [Expr]
fold_value stack token = exprFold0 tryParseValue stack token

fold_neg :: [Expr] -> String -> Maybe [Expr]
fold_neg stack token = exprFold1 tryParseNeg stack token

fold_plus :: [Expr] -> String -> Maybe [Expr]
fold_plus stack token = exprFold2 tryParsePlus stack token

fold_sub :: [Expr] -> String -> Maybe [Expr]
fold_sub stack token = exprFold2 tryParseSub stack token

fold_mult :: [Expr] -> String -> Maybe [Expr]
fold_mult stack token = exprFold2 tryParseMult stack token

fold_div :: [Expr] -> String -> Maybe [Expr]
fold_div stack token = exprFold2 tryParseDiv stack token

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

parse :: [String] -> Maybe [Expr]
parse tokens = do
  exprStack <- foldM build_somthing [] tokens
  return exprStack

evaluate :: [Expr] -> [Maybe Int]
evaluate exprs = map eval exprs

showMaybeInt :: Maybe Int -> String
showMaybeInt mx = maybe "Error Evaluating" show mx

main = do
  args <- getArgs
  let maybe_exprs = parse args
  if isNothing maybe_exprs then
    do
      putStrLn("Failed to parse")
      exitFailure
  else
    do
      mapM (putStrLn.showMaybeInt) (evaluate (fromJust maybe_exprs))
  exitSuccess
