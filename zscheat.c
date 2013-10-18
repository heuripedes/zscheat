#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#define MAX_CODES 4
#define MAX_CHEATS 1024
#define MAX_CHARS 18
#define MAX_LINE  1024

struct bin_cheat {
	uint8_t active;
	uint32_t code;
	
	uint8_t previous;
	uint8_t _dummy01;
	uint8_t _dummy02;
	char desc[MAX_CHARS+1];
	uint8_t _dummy03;
} __attribute__((packed));

static struct bin_cheat cheats[MAX_CHEATS] = {{0}};
static int num_cheats = 0;

static void eat_white(char **s) {
	char *c = *s;

	while (isspace(*c))
		c++;

	*s = c;
}

static void parse_line(char line[], int *ncodes, uint32_t *codes, char desc[]) {
	static int lineno = 0;
	char *c = line;

	*ncodes = 0;

	eat_white(&c);

	if (*c == '#' || *c == ';' || !strlen(c)) {
		fprintf(stderr, "notice: skipping line %i\n", lineno+1);
		lineno++;
		return;
	}

	while (*c && *c != ':') {
		if (*c == '+') {
			c++;
		} else {
			char *end;

			codes[(*ncodes)++] = strtol(c, &end, 16);

			if (end == c) {
				fprintf(stderr,
						"syntax error: invalid PAR code at "
						"line %i column %li\n",
						lineno + 1, c - line + 1);
				abort();
			}

			if (end - c != 8) {
				fprintf(stderr,
						"warning: expected 8 character PAR code at "
						"line %i column %li got %li\n",
						lineno + 1, c - line + 1,
						end - c);
			}

			c = end;
		}
		eat_white(&c);
	}

	if (*c != ':') {
		fprintf(stderr,
				"syntax error: missing ':' "
				"at line %i column %li: \n",
				lineno + 1, c - line + 1);
		abort();
	}

	c++;
	eat_white(&c);
	
	lineno++;

	strncpy(desc, c, MAX_CHARS);

	/* clear new lines */
	c = desc;
	while (*c && *c != '\n' && *c != '\r') {
		*c = toupper(*c);
		c++;
	}
	*c = 0;
}

static void put_cheat(int ncodes, uint32_t *codes, char desc[]) {
	struct bin_cheat *cheat = &cheats[num_cheats];

	if (ncodes == 0) {
		return;
	}

	int i;
	for (i = 0; i < ncodes; i++) {

		/* zsnes expects FE FC on the first cheat for some reason */
		if (num_cheats == 0) {
			cheat->_dummy01 = 0xfe;
			cheat->_dummy02 = 0xfc;
		}

		cheat->code = codes[i];
		strncpy(cheat->desc, desc, MAX_CHARS);
		num_cheats++;
		cheat++;
	}
}

static void parse(FILE *ifp) {
	char line[MAX_LINE];

	int ncodes = 0;
	uint32_t codes[MAX_CODES] = {0};
	char desc[MAX_CHARS] = {0};

	while (fgets(line, MAX_LINE, ifp)) {
		parse_line(line, &ncodes, codes, desc);
		put_cheat(ncodes, codes, desc);
	}
}

static void write_cheats(FILE *ofp) {
	fprintf(stderr, "%i cheats\n", num_cheats);
	fprintf(stderr, "%zu bytes written\n", 
			fwrite(cheats, sizeof(struct bin_cheat), num_cheats, ofp));
}

int main(int argc, char* argv[]) {
	FILE *fp;

	if (argc < 3) {
		printf("usage %s [input file] [output file]\n", argv[0]);
		return 0;
	}

	if ((fp = fopen(argv[1], "r")) == NULL) {
		perror("failed to open input file");
		abort();
	}

	parse(fp);
	fclose(fp);
	
	if ((fp = fopen(argv[2], "wb")) == NULL) {
		perror("failed to open input file");
		abort();
	}

	write_cheats(fp);

	fclose(fp);

	return 0;
}

