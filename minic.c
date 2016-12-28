#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#define NO_KEYWORDS 7
#define ID_LENGTH 12



enum tsymbol {
	tnull = -1,
	tnot, tnotequ, tmod, tmodAssign, tident, tnumber,
	tand, tlparen, trparen, tmul, tmulAssign, tplus,
	tinc, taddAssign, tcomma, tminus, tdec, tsubAssign,
	tdiv, tdivAssign, tsemicolon, tless, tlesse, tassign,
	tequal, tgreat, tgreate, tlbracket, trbracket, teof,
	tconst, telse, tif, tint, treturn, tvoid,
	twhile, tlbrace, tor, trbrace
};

struct tokenType {
	int number;
	union {
		char id[ID_LENGTH];

		int num;
	}value;
};
typedef struct tokenType tokenType;

char *keyword[NO_KEYWORDS] = {
	"const", "else", "if", "int", "return", "void", "while"
};
enum tsymbol tnum[NO_KEYWORDS] = {
	tconst, telse, tif, tint, treturn, tvoid, twhile
};

void lexicalError(int n)
{
	printf(" *** Lexical Error : ");
	switch (n) {
	case 1: printf("an identifier length must be less than 12.\n");
		break;
	case 2: printf("next character must be &.\n");
		break;
	case 3: printf("next character must be |.\n");
		break;
	case 4: printf("invalid character!!!\n");
		break;
	}
}

int superLetter(char ch)
{
	if (isalpha(ch) || ch == '_') return 1;
	else return 0;
}

int superLetterOrDigit(char ch)
{
	if (isalnum(ch) || ch == '_') return 1;
	else return 0;
}



struct tokenType scanner(FILE *fp)
{
	struct tokenType token;
	int i, index;
	char ch, id[ID_LENGTH];

	token.number = tnull;

	do {

		while (isspace(ch = getc(fp)));

		if (superLetter(ch)) {
			i = 0;
			do {
				if (i < ID_LENGTH) id[i++] = ch;
				ch = getc(fp);
			} while (superLetterOrDigit(ch));
			if (i >= ID_LENGTH) lexicalError(1);
			id[i] = '\0';
			ungetc(ch, fp);

			for (index = 0; index < NO_KEYWORDS; index++)
				if (!strcmp(id, keyword[index])) break;
			if (index < NO_KEYWORDS) {

				token.number = tnum[index];
				strcpy(token.value.id, id);
			}
			else {
				token.number = tident;
				strcpy(token.value.id, id);
			}
		}
		else if (isdigit(ch)) {
			token.number = tnumber;
			token.value.num = getIntNum(ch, fp);

		}
		else switch (ch) {
		case '/':
			ch = getc(fp);
			if (ch == '*')
				do {
					while (ch != '*') ch = getc(fp);
					ch = getc(fp);
				} while (ch == '/');
			else if (ch == '/')
				while (getc(fp) != '\n');
			else if (ch == '=') {
				token.number = tdivAssign;
				strcpy(token.value.id, "/=");
			}

			else {
				token.number = tdiv;
				strcpy(token.value.id, "/");
				ungetc(ch, fp);
			}
			break;
		case '!':
			ch = getc(fp);
			if (ch == '=') {
				token.number = tnotequ;
				strcpy(token.value.id, "!=");
			}
			else {
				token.number = tnot;
				strcpy(token.value.id, "!");
				ungetc(ch, fp);
			}
			break;
		case '%':
			ch = getc(fp);
			if (ch == '=') {
				token.number = tmodAssign;
				strcpy(token.value.id, "%=");
			}
			else {
				token.number = tmod;
				strcpy(token.value.id, "%");
				ungetc(ch, fp);
			}
			break;
		case '&':
			ch = getc(fp);
			if (ch == '&') {
				token.number = tand;
				strcpy(token.value.id, "&&");
			}
			else {
				lexicalError(2);
				ungetc(ch, fp);
			}
			break;
		case '*':
			ch = getc(fp);
			if (ch == '=') {
				token.number = tmulAssign;
				strcpy(token.value.id, "*=");
			}
			else {
				token.number = tmul;
				strcpy(token.value.id, "*");
				ungetc(ch, fp);
			}
			break;
		case '+':
			ch = getc(fp);
			if (ch == '+') {
				token.number = tinc;
				strcpy(token.value.id, "++");
			}
			else if (ch == '=') {
				token.number = taddAssign;
				strcpy(token.value.id, "+=");
			}
			else {
				token.number = tplus;
				strcpy(token.value.id, "+");
				ungetc(ch, fp);
			}
			break;
		case '<':
			ch = getc(fp);
			if (ch == '=') {
				token.number = tlesse;
				strcpy(token.value.id, "<=");
			}
			else {
				token.number = tless;
				strcpy(token.value.id, "<");
				ungetc(ch, fp);
			}
			break;
		case '=':

			ch = getc(fp);
			if (ch == '=') {

				token.number = tequal;
				strcpy(token.value.id, "==");

			}
			else {
				token.number = tassign;
				strcpy(token.value.id, "=");
				ungetc(ch, fp);
			}
			break;
		case '>':
			ch = getc(fp);
			if (ch == '=') {
				token.number = tgreate;
				strcpy(token.value.id, ">=");
			}
			else {
				token.number = tgreat;
				strcpy(token.value.id, ">");
				ungetc(ch, fp);
			}
			break;
		case '|':
			ch = getc(fp);
			if (ch == '|') {
				token.number = tor;
				strcpy(token.value.id, "||");
			}
			else {
				lexicalError(3);
				ungetc(ch, fp);
			}
			break;
		case '(': token.number = tlparen;
			strcpy(token.value.id, "(");
			break;
		case ')': token.number = trparen;
			strcpy(token.value.id, ")");
			break;
		case ',': token.number = tcomma;
			strcpy(token.value.id, ",");
			break;
		case ';': token.number = tsemicolon;
			strcpy(token.value.id, ";");
			break;
		case '[': token.number = tlbracket;
			strcpy(token.value.id, "[");
			break;
		case ']': token.number = trbracket;
			strcpy(token.value.id, "]");
			break;
		case '{': token.number = tlbrace;
			strcpy(token.value.id, "{");
			break;
		case '}': token.number = trbrace;
			strcpy(token.value.id, "}");
			break;
		case EOF: token.number = teof;
			strcpy(token.value.id, "EOF");
			break;
		default: {
			printf("Current character : %c", ch);
			lexicalError(4);
			break;
		}
		}
	} while (token.number == tnull);
	return token;
}

int getIntNum(char firstCharacter, FILE *fp)
{
	int num = 0;
	int value;
	char ch;

	if (firstCharacter != '0') {	//decimal
		ch = firstCharacter;
		do {
			num = 10 * num + (int)(ch - '0');
			ch = getc(fp);
		} while (isdigit(ch));
	}
	else {
		ch = getc(fp);
		if ((ch >= '0') && (ch <= '7'))	//octal
			do {
				num = 8 * num + (int)(ch - '0');
				ch = getc(fp);
			} while ((ch >= '0') && (ch <= '7'));
		else if ((ch == 'x') || (ch == 'X')) {	//hexa decimal
			while ((value = hexValue(ch = getc(fp))) != -1) {
				num = 16 * 16 + value;
			}
		}
		else {
			num = 0;
		}
	}
	ungetc(ch, fp); /*retract*/
	return num;
}

int hexValue(char ch) {
	switch (ch) {
	case '0': case '1': case '2': case '3': case '4': case'5': case '6': case'7': case'8': case'9':
		return (ch - '0');
	case 'A': case'B': case'C': case'D': case'E': case'F':
		return (ch - 'A' + 10);
	case 'a': case'b': case'c': case'd': case'e': case'f':
		return (ch - 'a' + 10);
	default:
		return -1;
	}
}

void main()
{

	tokenType token;
	FILE *fp;
	if ((fopen_s(&fp, "prime.mc", "r")) != 0)
	{
		printf("File open error!");
	}

	/* 소스코드(prime.c)에 대한 어휘분석기의 출력형태
	const int max = 100;
	*/

	do {
		token = scanner(fp);
		if (token.number == tnumber)     //토큰이 상수일 때
			printf("Token:%12d:(   %2d,%7d)\n", token.value.num, token.number, token.value.num);
		// (5) 출력형태: Token: 100 : (5, 100)
		else if (token.number == tident) //토큰이 명칭일 때
			printf("Token:%12s:(   %2d,%12s)\n", token.value.id, token.number, token.value.id);
		// (3) 출력형태: Token: max : (4, max)
		else if (token.number == teof)
			break;
		//토큰이 Keyword(명령어, 연산자) 일때
		else printf("Token:%12s:(   %2d,%7d)\n", token.value.id, token.number, 0);
		// (1) 출력형태: Token: const : (30, 0)
		// (2) 출력형태: Token: int : (33, 0)
		// (4) 출력형태: Token: = : (23, 0)
		// (6) 출력형태: Token: ;  (20, 0)
	} while (token.number != teof);



	getchar();
	fclose(fp);

}