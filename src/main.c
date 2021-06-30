#include <dirent.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

/* 
Copyright (c) 2021 Devine Lu Linvega

Permission to use, copy, modify, and distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE.
*/

#define NAME "Ten Digits"
#define DOMAIN "https://tendigits.space/"
#define LICENSE "https://github.com/tendigits/10d/blob/master/LICENSE"
#define SOURCE "https://github.com/tendigits/10d/edit/master"
#define INDEX "index.html"

struct dirent *dir;

typedef struct Lexicon {
	int len, refs[512];
	char files[512][64];
} Lexicon;

/* clang-format off */

char  clca(char c) { return c >= 'A' && c <= 'Z' ? c + ('a' - 'A') : c; } /* char to lowercase */
char  cuca(char c) { return c >= 'a' && c <= 'z' ? c - ('a' - 'A') : c; } /* char to uppercase */
int   slen(char *s) { int i = 0; while(s[i] && s[++i]) { ; } return i; } /* string length */
char *st__(char *s, char (*fn)(char)) { int i = 0; char c; while((c = s[i])) s[i++] = fn(c); return s; }
char *stuc(char *s) { return st__(s, cuca); } /* string to uppercase */
char *stlc(char *s) { return st__(s, clca); } /* string to lowercase */
char *scpy(char *src, char *dst, int len) { int i = 0; while((dst[i] = src[i]) && i < len - 2) i++; dst[i + 1] = '\0'; return dst; } /* string copy */
int   scmp(char *a, char *b) { int i = 0; while(a[i] == b[i]) if(!a[i++]) return 1; return 0; } /* string compare */
char *scsw(char *s, char a, char b) { int i = 0; char c; while((c = s[i])) s[i++] = c == a ? b : c; return s; } /* string char swap */
char *scat(char *dst, const char *src) { char *ptr = dst + slen(dst); while(*src) *ptr++ = *src++; *ptr = '\0'; return dst; } /* string cat */
int   ssin(char *s, char *ss) { int a = 0, b = 0; while(s[a]) { if(s[a] == ss[b]) { if(!ss[b + 1]) return a - b; b++; } else b = 0; a++; } return -1; } /* string substring index */
char *ccat(char *dst, char c) { int len = slen(dst); dst[len] = c; dst[len + 1] = '\0'; return dst; }

/* clang-format on */

int fpinject(FILE *f, Lexicon *l, char *filepath);

/* Print an error message */
int
error(char *msg, char *val)
{
	printf("Error: %s(%s)\n", msg, val);
	return 0;
}

/* Find a file */
int
findf(Lexicon *l, char *f)
{
	int i;
	char filename[64];
	scat(scsw(stlc(scpy(f, filename, 64)), ' ', '_'), ".htm");
	for(i = 0; i < l->len; ++i)
		if(scmp(l->files[i], filename))
			return i;
	return -1;
}

/* Print the "Edited on..." message */
void
fpedited(FILE *f, char *path)
{
	struct stat attr;
	stat(path, &attr);
	fputs("<em>", f);
	fprintf(f, "Edited on %s ", ctime(&attr.st_mtime));
	fprintf(f, "<a href='" SOURCE "/src/%s'>[edit]</a><br/>", path);
	fputs("</em>", f);
}

/* include a specific file */
int
fpportal(FILE *f, Lexicon *l, char *s, int head)
{
	int target;
	char srcpath[64], filename[64];
	target = findf(l, s);
	if(target < 0)
		return error("Missing portal", s);
	srcpath[0] = 0;
	filename[0] = 0;
	scat(scat(scat(srcpath, "inc/"), scpy(s, filename, 64)), ".htm");
	if(head)
		fprintf(f, "<h2 id='%s'><a href='#%s'>%s</a></h2>", scsw(filename, ' ', '_'), filename, s);
	fpinject(f, l, srcpath);
	l->refs[target]++;
	return 1;
}

/* Link template for terms */
int
fptemplate(FILE *f, Lexicon *l, char *s)
{
	int target;
	if(s[0] == '/')
		return fpportal(f, l, s + 1, 1);
	target = findf(l, s);
	if(target < 0)
		return error("Missing link", s);
	fprintf(f, "<a href='%s.html' class='local'>", scsw(stlc(s), ' ', '_'));
	fprintf(f, "%s</a>", scsw(stlc(s), '_', ' '));
	l->refs[target]++;
	return 1;
}

/* Do the file injection and swap out terms for links */
int
fpinject(FILE *f, Lexicon *l, char *filepath)
{
	FILE *inc;
	char c, s[1024];
	unsigned char t = 0;
	scsw(filepath, ' ', '_');
	if(!(inc = fopen(filepath, "r")))
		return error("Missing include", filepath);
	s[0] = 0;
	while((c = fgetc(inc)) != EOF) {
		if(c == '}') {
			t = 0;
			if(!fptemplate(f, l, s))
				return 0;
			continue;
		}
		if(c == '{') {
			s[0] = 0;
			t = 1;
			continue;
		}
		if(slen(s) > 1023)
			return error("Templating error", filepath);
		if(t)
			ccat(s, c);
		else
			fprintf(f, "%c", c);
	}
	fclose(inc);
	return 1;
}

/* Main content writer for HTML pages */
FILE *
build(FILE *f, Lexicon *l, char *name, char *srcpath)
{
	if(!f)
		return f;
	/* begin */
	fputs("<!doctype html><html lang='en' id='imghigh'>", f);
	fputs("<head>", f);
	fprintf(f,
		"<meta charset='utf-8'>"
		"<meta name='description' content='%s'/>"
		"<meta name='viewport' content='width=device-width,initial-scale=1'>"
		"<link rel='alternate' type='application/rss+xml' title='RSS Feed' "
		"href='../feed.xml' />"
		"<link rel='stylesheet' type='text/css' href='../links/main.css?c=20210630'>"
		"<title>" NAME " &mdash; %s</title>"
		"<link rel='shortcut icon' type='image/png' href='/favicon.ico'>",
		"An ongoing collection of notes for my projects and interests",
		name);
	fputs("</head>", f);
	fputs("<body id='imglow'>", f);
	fputs("<nav class='sitenav'>", f);
	fputs("<a href='index.html' title='Home'>", f);
	fputs("<svg xmlns='http://www.w3.org/2000/svg' viewBox='0 0 575 575'><path d='M68 183c0 49.7 40.3 90 90 90m-90-90c49.7 0 90 40.3 90 90m0-135c0 74.6 33.6 135 75 135m-75-135c41.4 0 75 60.4 75 135m30-165c-20 60-10 110 30 150m-30-150c30 30 40 80 30 150m105-135c-40 30-60 75-60 135m60-135c10 60-10 105-60 135m165-120c-50 20-85 65-105 135m105-135c-10 ", f);
	fputs("60-45 105-105 135m90 45c-60 20-100 50-120 90m120-90c-10 50-50 80-120 90m-15-90c-20 30-35 70-45 120m45-120c10 50-5 90-45 120m-30-150c-30 50-35 105-15 165m15-165c10 60 5 115-15 165m-75-150c30 50 40 95 30 135m-30-135c-20 50-10 95 30 135M98 318c0 60 20 100 60 120M98 318c40 20 60 60 60 120' fill='#333' stroke='#333' stroke-width='18' stroke-linecap='round' stroke-linejoin='round'/></svg></a>", f);
	fputs("<a href='index.html#notes'>Notes</a>", f);
	fputs("<a href='../feed.xml' title='Syndication feed'>RSS</a>", f);
	fputs("<a href='#imghigh' class='h'>+ Pixels</a>", f);
	fputs("<a href='#imglow' class='l'>- Pixels</a>", f);
	fputs("</nav>", f);
	fputs("<div class='wrap'>", f);
	if(!fpinject(f, l, srcpath))
		printf(">>> Building failed: %s\n", name);
	fputs("</div><footer><p>", f);
	fpedited(f, srcpath);
	fputs("Ten Digits © 2021 — ", f);
	fputs("<a href='https://creativecommons.org/licenses/by-nc/4.0/legalcode.txt'>CC BY-NC 4.0</a><br>", f);
	fputs("<a href='https://webring.xxiivv.com/#random'><svg fill='none' stroke-linecap='square' xmlns='http://www.w3.org/2000/svg' viewBox='0 0 300 300' stroke='#000' stroke-width='20' height='14' width='14'>", f);
	fputs("<path d='M201.962 210a60 60 0 10-103.924-60l-50 86.603'></path><path d='M98.038 210a60 60 0 10103.924-60l-50-86.603'></path><path d='M150 120a60 60 0 100 120h100'></path></svg> Find more handcrafted sites in the webring</a></p>", f);
	fputs("</footer>", f);
	fputs("</body></html>", f);
	return f;
}

/* Copy processed files into site folder */
int
generate(Lexicon *l)
{
	int i = 0;
	char srcpath[64], dstpath[64], filename[64];
	for(i = 0; i < l->len; ++i) {
		srcpath[0] = 0;
		dstpath[0] = 0;
		filename[0] = 0;
		/* src */
		scpy(l->files[i], filename, ssin(l->files[i], ".htm") + 1);
		scat(srcpath, "inc/");
		scat(srcpath, filename);
		scat(srcpath, ".htm");
		/* dst */
		scat(dstpath, "../site/");
		scat(dstpath, filename);
		scat(dstpath, ".html");
		fclose(build(fopen(dstpath, "w"), l, scsw(filename, '_', ' '), srcpath));
	}
	printf("Generated %d files\n", i);
	return 1;
}

/* Generate index */
int
index(Lexicon *l, DIR *d)
{
	while((dir = readdir(d)))
		if(ssin(dir->d_name, ".htm") > 0) {
			l->refs[l->len] = 0;
			scpy(dir->d_name, l->files[l->len++], 64);
		}
	closedir(d);
	printf("Indexed %d terms\n", l->len);
	l->refs[l->len] = 0;
	return 1;
}

/* Report any page not referenced */
void
inspect(Lexicon *l)
{
	int i;
	for(i = 0; i < l->len; ++i)
		if(!l->refs[i])
			error("Orphaned", l->files[i]);
}

/* primary function that runs everything */
int
main(void)
{
	Lexicon lex;
	DIR *d;
	lex.len = 0;
	if(!(d = opendir("inc")))
		return error("Open", "Missing inc/ folder. ");
	if(!index(&lex, d))
		return error("Indexing", "Failed");
	if(!generate(&lex))
		return error("Generating", "Failed");
	inspect(&lex);
	return 0;
}
