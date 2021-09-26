digit    [0-9]
letter   [A-Za-z]
%{
int count;
%}
%%
{letter}({letter}|{digit})*    count++;
%%
int yywrap(void) {
  return 1;
}
int main(void) {
    yylex();
    printf("nombre de variables = %d\n", count);
    return 0;
}
