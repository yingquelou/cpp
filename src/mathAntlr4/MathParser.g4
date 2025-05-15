parser grammar MathParser;
options{
	tokenVocab = MathLexer;
}
start: D+;