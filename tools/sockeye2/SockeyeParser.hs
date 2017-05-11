{-
  SockeyeParser.hs: Parser for Sockeye

  Part of Sockeye

  Copyright (c) 2017, ETH Zurich.

  All rights reserved.

  This file is distributed under the terms in the attached LICENSE file.
  If you do not find this file, copies can be found by writing to:
  ETH Zurich D-INFK, CAB F.78, Universitaetstr. 6, CH-8092 Zurich,
  Attn: Systems Group.
-}

module SockeyeParser
( parseSockeye ) where

import Control.Monad

import Text.ParserCombinators.Parsec as Parsec
import qualified Text.ParserCombinators.Parsec.Token as P
import Text.ParserCombinators.Parsec.Language (javaStyle)

import qualified SockeyeAST as AST

{- Setup the lexer -}
lexer = P.makeTokenParser (
    javaStyle  {
        {- list of reserved Names -}
        P.reservedNames = [
            "is", "are",
            "accept", "map",
            "over",
            "to", "at"
        ],

        {- valid identifiers -}
        P.identStart = letter,
        P.identLetter = alphaNum,

        {- comment start and end -}
        P.commentStart = "/*",
        P.commentEnd = "*/",
        P.commentLine = "//",
        P.nestedComments = False
    })

{- Helper functions -}
whiteSpace    = P.whiteSpace lexer
reserved      = P.reserved lexer
brackets      = P.brackets lexer
symbol        = P.symbol lexer
stringLiteral = P.stringLiteral lexer
commaSep      = P.commaSep lexer
commaSep1     = P.commaSep1 lexer
identifier    = P.identifier lexer
natural       = (P.natural lexer)
decimal       = P.decimal lexer

{- Sockeye parsing -}
sockeyeFile = do
    whiteSpace
    nodes <- many netSpec
    eof
    return $ AST.NetSpec $ concat nodes

netSpec = do
    nodeIds <- try single <|> multiple
    node <- nodeSpec
    return $ map (\nodeId -> (nodeId, node)) nodeIds
    where single = do
            nodeId <- nodeId
            reserved "is"
            return [nodeId]
          multiple = do
            nodeIds <- commaSep1 nodeId
            reserved "are"
            return nodeIds

nodeSpec = do
    a <- optionMaybe parseAccept 
    t <- optionMaybe parseTranlsate 
    overlay <- optionMaybe parseOverlay
    let accept = case a of Nothing -> []
                           Just blocks -> blocks
        translate = case t of Nothing -> []
                              Just maps -> maps
    return $ AST.NodeSpec accept translate overlay
    where parseAccept = do
            reserved "accept"
            brackets $ commaSep blockSpec
          parseTranlsate = do
            reserved "map"
            brackets $ commaSep mapSpec
          parseOverlay = do
            reserved "over"
            nodeId

mapSpec = do
    srcBlock <- blockSpec
    reserved "to"
    destNode <- nodeId
    dB <- optionMaybe parseDestBase
    let destBase = case dB of Nothing -> AST.base srcBlock
                              Just addr -> addr
    return $ AST.MapSpec srcBlock destNode destBase
    where parseDestBase = do
            reserved "at"
            addr

blockSpec = do
    base <- addr
    limit <- option base $ choice [parseLimit, parseLength base]
    return $ AST.BlockSpec base limit
    where parseLimit = do
            symbol "-"
            addr
          parseLength (AST.Addr base) = do
            symbol "/"
            b <- decimal
            return $ AST.Addr $ base + 2^b - 1

nodeId = do
    id <- identifier <?> "node identifier"
    return $ AST.NodeId id

addr = do
    addr <- natural <?> "address"
    return $ AST.Addr $ fromIntegral addr



parseSockeye :: String -> String -> Either ParseError AST.NetSpec
parseSockeye = parse sockeyeFile
