// konoha microbench by kimio kuramitsu

int testSimpleLoop() {
	int i;
	for(i = 0; i < 10_000_000; i++) {
	}
	return i;
}

int testString_indexof_string() {
	int i;
	s = "1234567890";
	key = "0";
	for(i = 0; i < 10_000_000; i++) {
		s.indexOf(key);
	}
	return i;
}

/*
int testString_indexof_regex() {
	int i;
	s = "1234567890";
	key = /0/;
	for(i = 0; i < 10_000_000; i++) {
		s.indexOf(key);
	}
	return i;
}
*/

int testString_indexof_empty() {
	int i;
	s = "1234567890";
	for(i = 0; i < 10_000_000; i++) {
		s.indexOf();
	}
	return i;
}

/*
int testString_replace_string() {
	int i;
	s = "1234567890";
	key = "0";
	for(i = 0; i < 10_000_000; i++) {
		s.replace(key, s);
	}
	return i;
}

int testString_replace_regex() {
	int i;
	s = "1234567890";
	key = /0/;
	for(i = 0; i < 10_000_000; i++) {
		s.replace(key, s);
	}
	return i;
}

int testString_replace_empty() {
	int i;
	s = "1234567890";
	for(i = 0; i < 10_000_000; i++) {
		s.indexOf(, s);
	}
	return i;
}
*/

int testString_getSize() {
	int i;
	s = "1234567890";
	key = /0/;
	for(i = 0; i < 10_000_000; i++) {
		x = s.size;
	}
	return i;
}

int testString_startsWith_string() {
	int i;
	s = "1234567890";
	for(i = 0; i < 10_000_000; i++) {
		s.startsWith(s);
	}
	return i;
}

int testString_startsWith_empty() {
	int i;
	s = "1234567890";
	for(i = 0; i < 10_000_000; i++) {
		s.startsWith();
	}
	return i;
}

int testString_endsWith_string() {
	int i;
	s = "1234567890";
	for(i = 0; i < 10_000_000; i++) {
		s.endsWith(s);
	}
	return i;
}

int testString_endsWith_empty() {
	int i;
	s = "1234567890";
	for(i = 0; i < 10_000_000; i++) {
		s.endsWith();
	}
	return i;
}

int testString_trim() {
	int i;
	empty10 = "          ";
	s = empty10 + "1234567890" + empty10;
	v = "";
	for(i = 0; i < 10_000_000; i++) {
		v = s.trim();
	}
	return i;
}

int testString_split_string() {
	int i;
	s = "0000000000";
	key = "0";
	String[] a = [];
	for(i = 0; i < 10_000_000; i++) {
		a = s.split(key);
	}
	return i;
}

int testString_split_regex() {
	int i;
	//  "1234567890";
	s = "0000000000";
	key = /0/;
	String[] a = [];
	for(i = 0; i < 10_000_000; i++) {
		a = s.split(key);
	}
	return i;
}


int testString_split_empty() {
	int i;
	//  "1234567890";
	s = "0000000000";
	String[] a = [];
	for(i = 0; i < 10_000_000; i++) {
		a = s.split();
	}
	return i;
}


float mops(int score) {
	if(score < 1) return 0;
	return (10000000 / score) / 100.0;
}

void print_data(String name, int score, float t) {
	bench = name + "[mops,ms]: "
	print bench, mops(score), t;
}

void run() {
	s = System.getTime();
	testSimpleLoop();
	t = System.getTime() - s;
	score = t;
	print_data("SimpleLoop", score, t);
	delta = t;
	//
	s = System.getTime();
	testString_indexof_string();
	t = System.getTime() - s;
	score = t - delta;
	print_data("String.indexOf(string)", score, t);
	/*
	s = System.getTime();
	testString_indexof_regex();
	t = System.getTime() - s;
	score = t - delta;
	print_data("String.indexOf(regex)", score, t);
	*/
	s = System.getTime();
	testString_indexof_empty();
	t = System.getTime() - s;
	score = t - delta;
	print_data("String.indexOf(null)", score, t);
	/*
	s = System.getTime();
	testString_replace_string();
	t = System.getTime() - s;
	score = t - delta;
	print_data("String.replace(string)", score, t);
	//
	s = System.getTime();
	testString_replace_regex();
	t = System.getTime() - s;
	score = t - delta;
	print_data("String.replace(regex)", score, t);
	//
	s = System.getTime();
	testString_replace_empty();
	t = System.getTime() - s;
	score = t - delta;
	print_data("String.replace(null)", score, t);
	*/
	s = System.getTime();
	testString_getSize();
	t = System.getTime() - s;
	score = t - delta;
	print_data("String.size", score, t);
	//
	s = System.getTime();
	testString_startsWith_string();
	t = System.getTime() - s;
	score = t - delta;
	print_data("String.startsWith(string)", score, t);
	//
	s = System.getTime();
	testString_startsWith_empty();
	t = System.getTime() - s;
	score = t - delta;
	print_data("String.startsWith(null)", score, t);
	//
	s = System.getTime();
	testString_endsWith_string();
	t = System.getTime() - s;
	score = t - delta;
	print_data("String.endsWith(string)", score, t);
	//
	s = System.getTime();
	testString_endsWith_empty();
	t = System.getTime() - s;
	score = t - delta;
	print_data("String.endsWith(null)", score, t);
	//
	s = System.getTime();
	testString_trim();
	t = System.getTime() - s;
	score = t - delta;
	print_data("String.trim", score, t);
	/*
	s = System.getTime();
	testString_split_string();
	t = System.getTime() - s;
	score = t - delta;
	print_data("String.split(string)", score, t);
	//
	s = System.getTime();
	testString_split_regex();
	t = System.getTime() - s;
	score = t - delta;
	print_data("String.split(regex)", score, t);
	//
	s = System.getTime();
	testString_split_empty();
	t = System.getTime() - s;
	score = t - delta;
	print_data("String.split(null)", score, t);
	*/

}

run();

