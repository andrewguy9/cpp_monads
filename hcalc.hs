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

eval :: Expr -> Maybe Int
eval (Val  n)   = Just n
eval (Neg  x  ) = eval x >>= safeNeg
eval (Plus x y) = join $ liftM2 safePlus (eval x) (eval y)
eval (Sub  x y) = join $ liftM2 safeSub  (eval x) (eval y)
eval (Mult x y) = join $ liftM2 safeMult (eval x) (eval y)
eval (Div  x y) = join $ liftM2 safeDiv  (eval x) (eval y)

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

exprFold0 :: (a -> Maybe b) -> a -> [b] -> Maybe [b]
exprFold0 f a bs = (:bs) <$> (f a)

exprFold1 :: (Expr -> String -> Maybe Expr) -> String -> [Expr] -> Maybe [Expr]
exprFold1 tryParse token (s1 : sx) = (:sx) <$> (tryParse s1 token)
exprFold1 tryParse token []        = Nothing

exprFold2 :: (Expr -> Expr -> String -> Maybe Expr) -> String -> [Expr] -> Maybe [Expr]
exprFold2 tryParse token (s1 : s2 : sx) = (:sx) <$> (tryParse s1 s2 token)
exprFold2 _ _ _                         = Nothing

parse_token :: [Expr] -> String -> Maybe [Expr]
parse_token stack token =
  (exprFold0 tryParseValue token stack)
  `mplus`
  (exprFold1 tryParseNeg token stack)
  `mplus`
  (exprFold2 tryParsePlus token stack)
  `mplus`
  (exprFold2 tryParseSub token stack)
  `mplus`
  (exprFold2 tryParseMult token stack)
  `mplus`
  (exprFold2 tryParseDiv token stack)

parse :: [String] -> Maybe [Expr]
parse tokens = foldM parse_token [] tokens

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
