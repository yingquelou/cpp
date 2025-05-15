lexer grammar MathLexer;
@lexer::header { 
	/* 
	pl
	 */
 }
@preinclude { 

	// #include"cppjson"
}
@lexer::postinclude { 
	// #include"cjson"
}
@lexer::members { 
	// using ok=type;
}
@lexer::declarations { 
	// using type=std::size_t;
	}
@lexer::definitions {
	struct MM{};
}
D: [0-9];
W: [a-zA-Z];
