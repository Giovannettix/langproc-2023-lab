%option noyywrap
%{
/* Now in a section of C that will be embedded
   into the auto-generated code. Flex will not
   try to interpret code surrounded by %{ ... %}*/

/* Bring in our declarations for token types and
   the yylval variable. */
#include "histogram.hpp"
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <sstream>
#include <vector>
// This is to work around an irritating bug in Flex
// https://stackoverflow.com/questions/46213840/get-rid-of-warning-implicit-declaration-of-function-fileno-in-flex
extern "C" int fileno(FILE *stream);

/* End the embedded code section. */
%}

%%

(-?[0-9]+((\.|\/)[0-9]+)?) {
   
   std::string numstr(yytext);
   char slash = '/';
   size_t position = numstr.find(slash);

   if(position != std::string::npos){
      std::string numerator = numstr.substr(0, position);
      std::string denominator = numstr.substr(position +1);
      yylval.numberValue = std::stod(numerator) / std::stod(denominator);
   }
   else{
      yylval.numberValue = atof(yytext);
   }

   return Number;

}

[a-zA-Z]+ {
   yylval.wordValue = new std::string(yytext);
   return Word;
}

(\[)[^\]]*(\]) {
   std::string text = std::string(yytext);
   std::string content = text.substr(1, text.size()-2);
   yylval.wordValue = new std::string(content);
   return Word;
}

.|\n       { fprintf(stderr, "Newline\n"); }


%%

/* Error handler. This will get called if none of the rules match. */
void yyerror (char const *s)
{
  fprintf (stderr, "Flex Error: %s\n", s); /* s is the text that wasn't matched */
  exit(1);
}
