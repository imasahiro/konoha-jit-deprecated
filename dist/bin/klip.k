#!/usr/local/bin/konoha



class Logger {

	OutputStream logfile;
	Boolean verbose;

	void createLogger(String filename, Boolean verbose)
	{
		_logfile = new OutputStream(filename, "a");
		_verbose = verbose;
		_logfile << "---------------------------------------" << EOL;
	}

	Logger getLogger() { return this; }

	void opAppend(String str)
	{
		log(str);
	}

	@Virtual void log (String str)
	{
		String logbuf = "";
		// TODO: we need Time functions
		// logbuf += "[" + Time.getDate() + "]";
		logbuf += str;
		if (_verbose) {
			OUT << logbuf << EOL;
		}
		_logfile << logbuf << EOL;
		_logfile.flush();
	}

}


class ErrorLogger extends Logger {

	@Override void log (String str)
	{
		String logbuf = "ERROR:";
		logbuf += str;
		if (_verbose) {
			ERR << logbuf << EOL;
		}
		_logfile << logbuf << EOL;
		_logfile.flush();
	}
			
}

EXIT_ERROR   = 0;
EXIT_SUCCESS = 1;
EXIT_NORMAL  = 2;
EXIT_WARN    = 3;

/**
 * 文字列のmodを行う
 * @param key 基準
 * @return 結果
 */
String String.modString(String key)
{
	int ofs = this.indexOf(key);
	if (ofs == -1) { return ""; }
	ofs += |key|;
	if (ofs >= this.getSize()) { return ""; }
	return this.subString(ofs,null);
}

/**
 * テキストファイルの内容をコピーする
 * @param infile コピー元ファイル名
 * @param outfile コピー先ファイル名
 */
void copyText(String infile ,String outfile)
{
    os.unlink(outfile);
    InputStream ins = new InputStream(infile,"r");
    OutputStream ous = new OutputStream(outfile,"w");
    foreach(String txt from ins) {
        ous.println(txt);
    }
    ous.close();
    ins.close();
}

/**
 * ２つのテキストファイルを比較して、
 * 対象側に無い行の文字列配列を返す
 * @param bef 比較の基準となるファイル名
 * @param ouf 比較の対象となるファイル名
 * @return 比較から得た差分の文字列配列を返す
 */
String[] diffText(String bef, String aft)
{
    String[] ret = new String[]();
    InputStream ins = new InputStream(bef,"r");
    boolean chk;
    foreach (String txt from ins) {
        chk = true;
        InputStream targ = new InputStream(aft,"r");
        foreach( String txt2 from targ ) {
            if( txt == txt2 ) { chk = false; }
		}
        targ.close();
        if (chk) { ret << txt; }
    }
    ins.close();
    return ret;
}

/**
 * SourceForgeのミラードメインをランダムに選択する
 */
String getSFMirror()
{
    String[] sites = ["keihanna" ,"jaist" ,"globalbase", "osdn", "iij"];
    return sites[Int.random(|sites|)];
}

/**
 * ユーザーにYes／Noの選択肢を選ばせる
 * @param message 選択肢に表示させるメッセージ文字列
 * @return 選択の結果(Yesならtrue)
 */
boolean askYorN(String message)
{

    OUT.print(%("!ASK : %s{0} [y/n] : ",message));
    while( true ){
        String? input = IN.readLine();
        if ( input == "y" ) {
            return true;
        } else if ( input == "n" ) {
            return false;
        } else {
            OUT.print("!ASK : please input [y/n] : ");
        }
    }
    return false;
}

/**
 * ディレクトリ内の全ファイルを削除する
 * (ただし、空のディレクトリは削除できないので残る)
 * @param dir 指定するディレクトリ名
 * @param files ディレクトリ内のファイル名の文字列配列
 */
void removeFiles(String dir,String[] files)
{
    foreach (String file from files) {
        String filepath = dir + "/" + file;
        if ( os.hasFile(filepath) ) {
            os.unlink(filepath);
        } else if ( os.hasDir(filepath) ) {
            String[] subfiles = os.listDir(filepath);
            removeFiles(filepath,subfiles);
        }
    }
}

int removeDirs(String dirpath,String removeto)
{
	Logger errlogger = ErrorLogger.getLogger();
    while (true) {
        if ( !os.hasDir(removeto) ) { break; }
        removeto += "/old";
    }
    if ( !os.rename(dirpath,removeto) ) {
		errlogger << "!ERR : Package.do_remove : rename";
        return EXIT_ERROR;
    }
    return EXIT_SUCCESS;
}


/**
 * システムの内部状態を確認し、klipの実行環境を作るクラス
 */
class State extends Logger {

	String klipdir;
	String tmpdir;
	String instdir;
	
	boolean locked;
	boolean chk_root;
	String[] ecode;

	State() {
		_ecode = new String[]();
	}
	
	void init()
	{
		initRoot();
		initDirs();
	}

	void initRoot()
	{
		boolean ret;
		String testdir = $konoha.package.path;
		ret = os.mkdir(testdir);
		if (ret) { os.unlink(testdir); }
		_chk_root = ret;
	}

	void initDirs()
	{
		String? platform = $konoha.os;
		if (platform == null) {
			_ecode << "e_plat";
		} else {
			if (platform == "windows") {
				initDirsWINDOWS();
			} else {
				initDirsUNIX();
				// or else?
			}
			makeDir(klipdir);
			makeDir(tmpdir);
			makeDir(instdir);
		}
	}

	void initDirsUNIX()
	{
		_klipdir = $env.HOME + "/.konoha/klip/";
		_tmpdir = _klipdir + "tmp/";

		if (!chk_root) { _instdir = $user.package.path; }
		else { _instdir = $konoha.package.path; }
		_instdir += "/";
	}

	void initDirsWINDOWS()
	{
		_klipdir = $konoha.path + "/klip/";
		_tmpdir = _klipdir + "tmp/";
		_instdir = $konoha.package.path;
		_instdir += "/";
	}

	void makeDir(String dirpath)
	{
		boolean ret = true;
		if (!os.hasDir (dirpath)) { ret = os.mkdir (dirpath); }
		if (!ret) { _ecode << "e_dirs"; }
	}

	Boolean lock(String lockfile)
	{
		String lockfilepath = _klipdir + lockfile;
		if (os.hasFile(lockfilepath)) {
			_ecode << "e_lock"; 
			_locked = false;
			return false;

		} else {
			OutputStream ous = new OutputStream(lockfilepath,"w");
			ous.close();
			_locked = true;
			return true;
		}
	}

	void release(String lockfile)
	{
		String lockfilepath = _klipdir + lockfile;
		if (_locked) { os.unlink(lockfilepath);	}
	}

	boolean isFailed()
	{
		return (_ecode.getSize() > 0);
	}

    /**
     * ecodeを読み、対応するエラーメッセージを表示する
     */
    void info()
    {
		Logger logger = Logger.getLogger();
		Logger errlogger = ErrorLogger.getLogger();
        if ("e_dirs" in _ecode) {
			errlogger << "!ERR : failed to create directories";
        }
        if ("e_plat" in _ecode) {
			errlogger << "!ERR : cannot detect your platform";
        }
        if ("e_lock" in _ecode) {
			logger << "INFO : klip is locked now";
        }
    }

}

/**
 * json形式のパッケージリストをパースするクラス
 */
class JsonReader {

	String file;

	JsonReader(String file) {
		this.file = file;
	}

	String[] getNameList()
	{
		String[] ret = new String[]();
		String[] box;
		String raw = rawjs();
		String check = '"name":"';
		box = raw.split(check,0);
		foreach(String txt from box) {
			ret << txt.split('"',0)[0];
		}
		ret.remove(0);
		return ret;
	}

	/** 
	 * 自身のフィールドの情報から、
	 * jsonの書式に沿ったパッケージ情報を生成する
	 * @return パッケージ情報のjson書式での文字列
	 */
	String generate(String name,String version,String url,String[] depend)
	{
		String ret;
		ret += "{\n";
		ret += setNode("name",name) + ",\n";
		ret += setNode("version",version) + ",\n";
		ret += setNode("url",url);
		if ( depend.getSize() > 0 ) {
			ret += ",\n" + arrayNode("depend",depend);
		}
		return ret + "\n}\n";
	}

	String[] getElements(String name,String elemname)
	{
		String[] ret;
		String str = getElement(name,elemname);
		ret =  str.split("&",0);
		return ret;
	}


	/** 
	 * リストファイルから各フィールドの情報を抜き出す
	 * @param elemname リクエストするフィールド
	 * @return リスト内から抜き出したフィールド情報の文字列
	 */
	String getElement(String name,String elemname)
	{
		String ret;
		String raw = rawjs();
		String check = %('"name":"%s{0}"',name);
		boolean trig = true;
		while( trig ) {
			if ( raw.split(",",0)[0].opHas(check) ) {
				return parseThisTree(raw,elemname); 
			}
			raw = succRaw(raw);
			trig = ( |raw| != 0 );
		}
		return ret;
	}

	/** 
	 * リストファイル内での目的のパッケージの位置、
	 * 指定されたフィールド部分を抜き出す
	 * @param raw 目的のパッケージ情報以降のリストの全文字列
	 * @param elemname リクエストするフィールド
	 * @return リスト内から抜き出したフィールド情報の文字列
	 */
	String parseThisTree(String raw,String elemname)
	{
		String ret;
		String key = %('"%s{0}":',elemname);
		boolean trig = true;
		while(trig) {
			String pic = raw.split(",",0)[0];
			if ( pic.opHas(key) ) { 
				if ( elemname != "depend" ) {
					ret = pic.split('":"',0).pop();
					ret = ret.split('"',0)[0];
					return ret;
				} else {
					return getDependencies(raw);
				}
			} else if ( pic.opHas("}") && !pic.opHas("{0}") ) {
				break;
			}
			raw = succRaw(raw);
			trig = ( |raw| != 0 );
		}
		return ret;
	}

	/** 
	 * パッケージの依存情報を、単純に記号連結した文字列として出力する
	 * @param raw 目的のパッケージの依存情報以降のリストの全文字列
	 * @return 連結した依存情報の文字列
	 */
	String getDependencies(String raw)
	{
		String ret;
		String raws = raw - "]";
		String base = raws.modString('"depend":["'); 
		while( base != "") {
			ret += base.split('"',0)[0] + "&";
 			base = base.modString('","');
		}
		return ret;
	}

	/** 
	 * リストファイルのテキストを全て連結し、
	 * 改行文字を消去した文字列で返す
	 * @return 連結したテキストの文字列
	 */
	String rawjs()
	{
		String ret;
		InputStream ins = new InputStream(file,"r");
		foreach( Any iput from ins ) {
			String txt = (String)iput;
			ret += txt;
		}
		ins.close();
		return ret - "\t";
	}

	/** 
	 * jsonの区切り文字(,)単位で現在の読み取り位置を進める
	 * @param raw 現在の読み取り位置以降の文字列
	 * @return １単位進めた位置以降の文字列
	 */
	String succRaw (String raw)	{
		return raw = raw.modString(",");
	}

	/** 
	 * jsonのノードの文字列を生成する
	 * @param name ノード名
	 * @param data ノードのデータ
	 * @return 生成したノードの文字列
	 */
	String setNode(String name,String data)
	{
		return %('"%s{0}":"%s{1}"',name,data );
	}

	/** 
	 * jsonの配列ノードの文字列を生成する
	 * @param name ノード名
	 * @param data ノードのデータ配列
	 * @return 生成した配列ノードの文字列
	 */
	String arrayNode(String name,String[] data)
	{
		String ret;
		ret += %('"%s{0}":[',name);
		foreach ( String cell from data) {
			ret += %('"%s{0}",',cell);
		}
		return ret[ to (|ret|-2) ] + "]";
	}

}


String DictMap.toValues(String[] fields)
{
	String ret = "";
	foreach (String fld in fields) {
		if (this[fld] == null) {
			this[fld] = "";
		}
		ret += %('%s{0},',%k(this[fld]));
	}
	if (|ret| > 0) {
		int len = |ret| - 1;
		ret = ret[0 until len];
	}
	ret = %("VALUES(%s{0})",ret);
	return ret;
}

class SqliteTable {

	Connection con;
	String name;
	boolean initialized;

	String[] fields;
	String[] indexes;

	SqliteTable(Connection con, String name)
	{
		_con = con;
		_name = name;
		_initialized = false;
	}

	void initAsExistedTable()
	{
		if (!initialized) {
			_fields  = getFields();
			_indexes = getIndexes();
			_initialized = true;
		}
	}

	void initAsNewTable(String[] fldnames)
	{
		if (!initialized && (|fldnames| > 0)) {
			_fields = new String[]();
			String msg;
			foreach (String fldnm in fldnames) {
				msg += %('"%s{0}",',fldnm);
				_fields << fldnm;
			}
			int len = |msg| - 1;
			msg = msg[ until len];
			msg = %("CREATE TABLE %s{0}(%s{1})",name,msg);
			_con.exec(msg);
			initialized = true;
		}
	}

	boolean isInitialized()
	{
		if (!initialized) {
			String msg = "[sqlite] table is not initialized : %s{0}";
			ERR << %(msg,name) << EOL;
		}
		return initialized;
	}


	@Private String[] getFields()
	{
		String[] ret = new String[]();
		ResultSet rs = _con.query(%("SELECT * FROM %s{0}",name));
		int i;
		for (i = 0 ; i < rs.getSize() ; i++) {
			ret << rs.getName(i);
		}
		rs.close();
		return ret;
	}

	@Private String[] getIndexes()
	{
		String[] ret = new String[]();
		String place = %("type='index' AND tbl_name='%s{0}'",name);
		String msg = %("SELECT name FROM sqlite_master WHERE %s{0}",place);
		ResultSet rs = _con.query(msg);
		while (rs.next()) {
			ret << rs.getString("name");
		}
		rs.close();
		return ret;
	}

	void addIndex(String idxnm,String field)
	{
		if (isInitialized()) {
			if ( field in fields && !(idxnm in indexes)) {
				String msg;
				msg = "CREATE INDEX %s{0} ON %s{1}(%s{2})";
				msg = %(msg,idxnm,name,field);
				_con.exec(msg);
				_indexes << idxnm;
			}
		}
	}

	DictMap[] getValues(String sel,String whr)
	{
		DictMap[] ret;
		if (isInitialized()) {
			ret  = new DictMap[]();
			String msg = %("SELECT %s{0} FROM %s{1} %s{2}",sel,_name,whr);
			ResultSet rs = _con.query(msg);
			while (rs.next()) {
				DictMap dict = {};
				foreach (String fld in _fields) {
					if (rs.get(fld) != null) {
						dict[fld] = rs.get(fld);
					}
				}
				ret << dict;
			}
			rs.close();
		}
		return ret;
	}

	void addValues(DictMap vals)
	{
		if (isInitialized()) {
			String values = vals.toValues(_fields);
			String msg = %("INSERT INTO %s{0} %s{1}",_name,values);
			_con.exec(msg);
		}
	}

	void updateValues(String exp, String? place)
	{
		if (isInitialized()) {
			String msg;
			if (place == null) {
				msg = "UPDATE %s{0} SET %s{1}";
				msg = %(msg,_name,exp);
			} else {
				msg = "UPDATE %s{0} SET %s{1} WHERE %s{2}";
				msg = %(msg,_name,exp,place);
			}
			con.exec(msg);
		}
	}

	void delValues(String? place)
	{
		if (isInitialized()) {
			String msg;
			if ( place == null ) {
				msg = %("DELETE FROM %s{0}",_name);
			} else {
				msg = 'DELETE FROM %s{0} WHERE %s{1}';
				msg = %(msg,_name,place);
			}
			con.exec(msg);
		}
	}

}


class SqliteHandler {

	Connection con;
	SqliteTable[] tables;

	/**
	 * SqliteHandlerのコンストラクタ
	 * @param file sqlite3のDBファイル名
	 */
	SqliteHandler(String file)
	{
		String db = "sqlite:" + file;
		_con = new Connection(db);
		_tables = new SqliteTable[]();
		initTables();
 	}

	void initTables()
	{
		String msg;
		msg = "SELECT name FROM sqlite_master WHERE type='table'";
		ResultSet rs = _con.query(msg);
		while (rs.next()) {
			String tblname = rs.getString("name");
			SqliteTable tbl = new SqliteTable(_con,tblname);
			tbl.initAsExistedTable();
			_tables << tbl;
		}
		rs.close();
	}

	SqliteTable? getTable(String table)
	{
		SqliteTable? ret;
		foreach (SqliteTable tbl from tables) {
			if (tbl.getName() == table) {
				ret = tbl;
				break;
			}
		}
		return ret;
	}

	boolean hasTable(String table)
	{
		return (getTable(table) != null);
	}

	void addTable(String table, String[] flds)
	{
		if (!hasTable(table)) {
			SqliteTable tbl = new SqliteTable(_con,table);
			tbl.initAsNewTable(flds);
			_tables << tbl;
		}
	}

	void addIndex(String table,String idxnm,String field)
	{
		SqliteTable? tbl = getTable(table);
		if (tbl != null) {
			tbl.addIndex(idxnm,field);
		}
	}
	
	DictMap[] getValues(String table,String? field,String? place)
	{
		DictMap[] ret;
		SqliteTable? tbl = getTable(table);
		if (tbl != null) {
			String whr;
			if (place != null) { whr = "WHERE " + place; }
			String fld = "*";
			if ( field != null ) { fld = field; }
			ret = tbl.getValues(fld,whr);
		}
		return ret;
	}

	void addValues(String table, DictMap vals)
	{
		SqliteTable? tbl = getTable(table);
		if (tbl != null) {
			tbl.addValues(vals);
		}
	}

	void updateValues(String table, String exp, String? place)
	{
		SqliteTable? tbl = getTable(table);
		if (tbl!= null) {
			tbl.updateValues(exp,place);
		}
	}

	void delValues(String table, String? place)
	{
		SqliteTable? tbl = getTable(table);
		if (tbl != null) {
			tbl.delValues(place);
		}
	}

}


class PackageList {
	
	@Virtual DictMap getMember(String pkgname);

	@Virtual String[] getPackageNameList();
	
}

class SqliteDB extends PackageList {

    SqliteHandler sh;

    SqliteDB(String filename) {
        this.sh = new SqliteHandler(filename);
    }

    DictMap getMember(String pkgname)
    {
        DictMap ret;
        ret = getPackageMember(pkgname);
        String[] depend = getPackageDependency(pkgname);
        ret["depend"] = depend;
        return ret;
    }

    DictMap getPackageMember(String pkgname)
    {
		DictMap ret;
		try {
			String place = %("name='%s{0}'",pkgname);
			DictMap[] maps = sh.getValues("package",null,place);
			ret = maps[0];
		} catch (OutOfIndex!! oe) {}
		return ret;
    }

    String[] getPackageDependency(String pkgname)
    {
        String[] ret = new String[]();
        String place = %("name='%s{0}'",pkgname);
        DictMap[] maps = sh.getValues("dependency",null,place);
        foreach (DictMap map in maps) {
            String buf = "%s{0}=%s{1}";
            ret << %(buf,map["dname"],map["dver"]);
        }
        return ret;
    }

	String[] getPackageNameList()
	{
		String[] ret;
		try {
			ret = new String[]();
			DictMap[] maps = sh.getValues("package","name",null);
			foreach (DictMap map in maps) { ret << map["name"]; }
		} catch (Null!! ne) {}
		return ret;
	}

}


class JsonFile extends PackageList {

	JsonReader jr;

	JsonFile(String file) {
		this.jr = new JsonReader(file);
	}

	DictMap getMember(String name)
	{
		DictMap ret = {};
		ret["version"] = jr.getElement(name,"version");
		ret["url"]     = jr.getElement(name,"url");
		ret["depend"]  = jr.getElements(name,"depend");
		return ret;
	}

	String[] getPackageNameList()
	{
		String[] ret;
		ret = jr.getNameList();
		return ret;
	}

	/**
	 * json形式のテキストの内容をリセットする
	 * @param filename リセットするテキストファイル名
	 */
	void clearJson(String filename)
	{
		if ( filename.split(".",0).pop() == "json" ) {
			os.unlink(filename);
			OutputStream ous = new OutputStream(filename,"w");
			ous.println("{\n}");
			ous.close();
		}
	}

}


class ListFactory {

	PackageList getPackageList(String filename)
	{
		Logger errlogger = ErrorLogger.getLogger();
		PackageList ret;
		String ext = filename.split(".",0).pop();
		if (ext == "db") {
			try {
				ret = new SqliteDB(filename);
			} catch (Exception e) {
				errlogger << "failed to connect with SQLite3 database";
			}
		} else if (ext == "json") {
			ret = new JsonFile(filename);
		} else {
			errlogger << "cannot detect filetype of PackageList";
		}
		return ret;
	}

}


/**
 * キャッシュファイルを管理するクラス
 */
class Cache {

	String latest;
	String curdata;
	String prevdata;
	String pkgdata;
	String dataurl;

	Cache(String dir, String dataurl, String pkgdata) {
		_curdata  = dir + "current.json";
		_prevdata = dir + "previous.json";
		_latest   = dir + "latest.log";
		_pkgdata  = dir + "" + pkgdata;
		_dataurl  = dataurl + "/" + pkgdata;
 	}

    /**
     * システムファイルが無い場合に新規作成する
     */
    void init()
    {
		if (!os.hasFile(_latest)) { clearLatest(); }

		if (!os.hasFile(_curdata)) { JsonFile.clearJson(_curdata); }

		if (!os.hasFile(_prevdata)) { JsonFile.clearJson(_prevdata); }
    }

	void removeAll()
	{
		os.unlink(_latest);
		os.unlink(_curdata);
		os.unlink(_prevdata);
		os.unlink(_pkgdata);
	}

	//latest

	/** 
	 * latestの特定セクションを更新する
	 * @param section 変更するセクション名
	 * @param diff 書き換える内容
	 */
	void updateLatest(String section, String diff)
	{
		boolean flag = false;
		InputStream ins = new InputStream(_latest, "r");
		OutputStream ous = new OutputStream(_latest + "~", "a");
		String newtxt = section + diff;
		foreach (String txt from ins) {
			if (txt.startsWith(section)) {
				flag = true;
				txt = newtxt;
			}
			ous.println(txt);
		}
		if (!flag) { ous.println (newtxt); }
		ous.close();
		ins.close();
		os.unlink(_latest);
		os.rename(_latest + "~", _latest);
	}

	/** 
	 * latestの全セクションリセットする
	 */
	void clearLatest() {
		os.unlink(_latest);
		OutputStream ous = new OutputStream(_latest,"w");
		ous.close();
		updateLatest("Last-Installed-Package:","None");
		updateLatest("Last-Modified:","None");
	}

	//pkgdata

	/** 
	 * pkgdataをサーバーから取得する
	 * @return 取得の成否
	 */
	boolean getKlipdata()
	{
		boolean ret;
		Logger logger = Logger.getLogger();
		Logger errlogger = ErrorLogger.getLogger();

		logger << "KLIP : checking packagedata...";
		if (os.hasFile(_pkgdata)) {
			if (!needtoUpdateList()) {
				logger << "KLIP : there is nothing to change at server";
				return true;
			}
			logger << "KLIP : there are some changes at server";
			os.unlink(_pkgdata);
		}

		// TODO : are there any method to set socket timeout??
		// it will block when it can create socket, cannot find server tough;
		logger << "KLIP : update packagedata...";
		InputStream ins = new InputStream(_dataurl, "rb");
		OutputStream ous = new OutputStream(_pkgdata, "wb");
		int b = ins.getChar();
		if (b == -1) {
			errlogger << "failed to create socket for sourcefoge.jp";
			return false;
		}
		while (true) {
			ous.putChar(b);
			b = ins.getChar();
			if (b == -1) { break; }
		}
		ins.close();
		ous.close();
		return ret;
	}

	boolean needtoUpdateList()
	{
		String curr = getLastModified();
		String late = getLastModifiedFromUrl();
		if (curr != late) { updateLatest("Last-Modified:",late); }
		return (curr != late);
	}

	String getLastModified()
	{
		String ret;
		InputStream ins = new InputStream(_latest);
		foreach (String txt in ins) {
			if (txt.startsWith("Last-Modified:")) {
				int idx = txt.indexOf(":") + 1;
				ret = txt[idx to ];
			}
		}
		return ret;
	}

	String getLastModifiedFromUrl()
	{
		String ret;
		String domain = _dataurl.replace("http://","");
		String host = domain.split("/",0)[0] + "##" - "##";
		String path = domain.replace(host,"");
		Socket s = new Socket(host,80);
		OutputStream ous = s.getOutputStream();
		ous << %("GET %s{0} HTTP/1.0\n",path);
		ous << %("Host: %s{0}:80\n",host);
		ous << "\n";
		ous.flush();
		InputStream ins = s.getInputStream();
		while (true) {
			String? head = ins.readline();
			if (head == null) {
				break;
			} else if (head.startsWith("Last-Modified:")) {
				ret = head.replace("Last-Modified:","");
				break;
			}
		}
		ous.close();
		ins.close();
		s.close();
		return ret;
	}

	//curdata & prevdata

	/** 
	 * curdataの一時的なバックアップファイルを作成する
	 */
	void mkBufCurData()
	{
		copyText(_curdata,_curdata + "~");
	}

	/** 
	 * curdataのバックアップファイルを削除する
	 */
	void rmBufCurData()
	{
		os.unlink(_curdata + "~");
	}

	/** 
	 * curdataのバックアップの内容でprevdataを書き換える
	 */
	void cpBufCur2Prev()
	{
		copyText(_curdata + "~",_prevdata);
	}

	/** 
	 * curdataの内容を更新する
	 * @param data 更新するパッケージ情報の文字列配列
	 * @param mode 更新のモード
	 */
	void modCurData(String[] data,String mode)
	{
		InputStream ins = new InputStream(_curdata,"r");
		OutputStream ous = new OutputStream(_curdata + "#","w");
		String base = %('"name":"%s{0}"',data[0]);
		String jsform = '{"name":"%s{0}","version":"%s{1}","url":"%s{2}"},';
		String newdata = %(jsform,data[0],data[1],data[2]);
		if (mode == "a") {
			addPkgData(ins,ous,data);
		} else {
			foreach (String txt from ins) {
				if ((mode == "r") && txt.opHas(base)) {
					ous.println(newdata);
				} else if (!(base in txt)) {
					ous.println(txt);
				}
			}
		}
		ins.close();
		ous.close();
		os.unlink(_curdata);
		os.rename(_curdata + "#",_curdata);
	}

	/** 
	 * 入力ストリームからのテキストを読み取って、
	 * 更に新しいパッケージの情報文字列を追加して出力ストリームに出力する
	 * @param ins 入力ストリーム
	 * @param ous 出力ストリーム
 	 * @param put 新しく挿入するパッケージ情報の文字列
	 */
	void addPkgData(InputStream ins, OutputStream ous, String[] data)
	{
		String name = data[0];
		String jsform = '{"name":"%s{0}","version":"%s{1}","url":"%s{2}"},';
		String newdata = %(jsform,data[0],data[1],data[2]);
		foreach (String txt from ins) {
			if (txt == "}") { break; }
			String txtname = txt.modString('"name":"').split('"',0)[0];
			if (txtname > name && newdata != "") {
				ous.println(newdata);
				newdata = "";
			}
			ous.println(txt);
		}
		if (newdata != "") { ous.println(newdata); }
		ous.println("}");
	}

}

class Package
{
	State state;

	/** パッケージの名前 */
	String name;
	/** パッケージのバージョン */
	String version;
	/** パッケージのURL */
	String url;
	/** パッケージが依存するパッケージの情報 */
	String[] depend;

	/**
	 * Packageクラスのコンストラクタ
	 * @param name パッケージの名前
	 */
	Package(State state,String name)
	{
		_state = state;
		_name = name;
	}

	/**
	 * パッケージの名前をDictMapから取得する
	 * @param map このパッケージの情報を持つDictMap
	 */
	void setMember(DictMap map)
	{
		try {
			_version = map["version"];
			_url = map["url"];
			_depend = map["depend"];
		} catch (Null!! ne) {
			//set as force
		}
	}
	
	//operations
	
	/**
	 * パッケージをインストールする
	 * @return インストールの結果
	 */
	int install()
	{
		Logger logger = Logger.getLogger();
		logger << %("KLIP : install ... : %s{0}\tver=%s{1}",name,version);
		if( !askYorN("install it?") ) { 
			logger << "KLIP : denyed to install";
			return EXIT_NORMAL;
		} else {
			logger << "KLIP : install this package ...";
		}
		return do_install();
	}
		
	/**
	 * パッケージを更新する
	 * @return 更新の結果
	 */
	int update()
	{
		Logger logger = Logger.getLogger();
		Logger errlogger = ErrorLogger.getLogger();
		logger << %("KLIP : update  : %s{0}\tver=%s{1}",name,version);
		if( !askYorN("update it?") ) {
			logger << "KLIP : denyed to update";
			return EXIT_NORMAL;
		} else {
			logger << "KLIP : update this package ...";
		}
		if ( do_remove() == EXIT_ERROR ) {
			errlogger << " Package.update : do_remove";
			return EXIT_ERROR;
		}
		return do_install();
	}

	/**
	 * パッケージを削除する
	 * @return 削除の結果
	 */
	int remove()
	{
		Logger logger = Logger.getLogger();
		logger << %("KLIP : remove  : %s{0}\tver=%s{1}",name,version);
		if( !askYorN("remove it?") ) {
			logger << "KLIP : denyed to remove";
			return EXIT_NORMAL;
		} else {
			logger << "KLIP : remove this package ...";
		}
		return do_remove();
	}

	/**
	 * ファイルの大きさを返す
	 * @return size
	 */
	int getfilesize(String filename) {
		int size;
		InputStream ins = new InputStream(filename, "rb");
		char ch = ins.getChar();
		while (ch != -1) {
			ch = ins.getChar();
			size++;
		}
		print size;
		ins.close();
		return size;
	}

	/**
	 * インストールにおける実際のファイル操作を行う
	 * @return 操作の結果
	 */
	int do_install()
	{
		String filepath = state.tmpdir + url.split("/",0).pop();
		Logger logger = Logger.getLogger();
		Logger errlogger = ErrorLogger.getLogger();
		if (!os.hasFile(filepath)) {
			/* if theres no file */
			if (!download(filepath)) {
				errlogger << " Package.do_install : download";
				return EXIT_ERROR;
			}
		} else {
			logger << "INFO : archieve tmpfile detected";
			/* check tmp file corrects */
			if (!(getfilesize(filepath) > 0)) {
				/* filesize is zero. exit with ERROR */
				errlogger << %("illegal filesize %s{0}\n remove %s{0} first.", filepath);
				return EXIT_ERROR;
			}
		}
		boolean ret = decompress(filepath);
		if (!ret) {
			errlogger << " Package.do_install : decompress";
			return EXIT_ERROR;
		}
		logger << "INFO : done!";
		return EXIT_SUCCESS;
	}

	/**
	 * 削除における実際のファイル操作を行う
	 * @return 操作の結果
	 */
	int do_remove()
	{
		int ret;
		String pkgdir = state.instdir + name;
		Logger errlogger = ErrorLogger.getLogger();
		String[] files = os.listDir(pkgdir);
		if (|files| == 0) {
			errlogger << " Package.do_remove : NOT FOUND";
			return EXIT_ERROR;
		}
		removeFiles(pkgdir,files);
		ret = removeDirs(pkgdir,state.tmpdir + name);
		return ret;
	}

	/**
	 * パッケージのファイル（アーカイブ）を取得する
	 * @param filepath ファイルの保存パス
	 * @return 取得の成否
	 */
	boolean download(String filepath)
	{
		boolean ret = true;
		String pkgurl = %(url,getSFMirror());
		InputStream ins = new InputStream(pkgurl);
		OutputStream ous = new OutputStream(filepath,"wb");
		int buf = 0;
		while(true) {
			buf = ins.getChar();
			if ( buf == -1 ) { break; }
			ous.putChar(buf);
		}
		ins.close();
		ous.close();
		return ret;
	}
	
	/**
	 * パッケージのファイル（アーカイブ）を解凍する
	 * @param filepath アーカイブの存在するパス
	 * @return 解凍の成否
	 */
	boolean decompress(String filepath)
	{
		boolean ret = true;
		Logger errlogger = ErrorLogger.getLogger();
		String cmd = "-xfz " + filepath;
		int res;
		if ($konoha.os == "windows") {
			eval("using tar.*;");
			eval(%("Tar.tar('%s{0}');", cmd));
		} else {
			res = system("tar " + cmd);
		}
		if (res != 0) {
			errlogger << " Package.decompress: cannot decompress";
			ret = false;
		} else {
			ret = os.rename(name,state.instdir + name);
			if (!ret) {
				errlogger << " Package.decompress : System.rename";
			}
		}
		return ret;
	}

}

/********** GLOBAL VARIABLES ***********/
/* Version */
VERSION = 0;
MAJOR = 1;
MINOR = 14;
LASTUPDATE = "09/10/11";

/* operation info */
NOARG = 0;
HASARG = 1;

DATAFILE = "klip.db";
LOCKFILE = "klip.lock";
KLIPURL = "http://konoha.sourceforge.jp/klip";
LOGFILE = "klip.log"
ERRLOGFILE = "klip.error";
SUCCESS_INFO = """
INFO : Package has installed to [%s{0}].
       But to make sure,
       you should rebuild downloaded package
""";

operations = {
	install:
	{
		description: "install package",
		has_arg: 1
		//action: Klip.isInstalled;  /* its convenient if we can write like this  */
	},
	update:
	{
		description: "update package",
		has_arg: 1
	},
	remove:
	{
		description: "remove package",
		has_arg: 1
	},
	search:
	{
		description: "search package from the list on sourceforge",
		has_arg: 1
	},
	list:
	{
		description: "list packages on the server",
		has_arg: 0
	},
	installed:
	{
		description: "list packages that already installed",
		has_arg: 0
	},
	undo:
	{
		description: "undo previous operation",
		has_arg: 0
	},
	env :
	{
		description: "show klip environment variables",
		has_arg: 0
	}
};

/********** GLOBAL VARIABLES END***********/

void klip_show_usage()
{
	OUT <<  %("""
*************************************
*  klip ( Konoha LIbrary Provider ) *
*         version %d{0}.%d{1}.%d{2} (%s{3}) *
*************************************
Usage:
  klip operation [pkg]

Operations:
""", VERSION, MAJOR, MINOR, LASTUPDATE) << EOL;
	foreach (key, value in operations) {
		if (value["has_arg"] == 0) {
			OUT << %("%s{0}\t\t: %s{1}", key, value["description"])  << EOL;
		} else {
			OUT << %("%s{0} (pkg) \t: %s{1}", key, value["description"]) << EOL;
		}
	}
}

/*-i/install pkg : install package
  -u/update  pkg : update package
  -r/remove  pkg : remove package
  -s/search  pkg : search  package from the list on sourceforge
  -l/list        : list packages on the server
  installed      : list packages that already installed
  undo           : undo previous operation
  env            : show klip environment variables
""";
*/





/**
 * Klipのメインクラス
 */
class Klip {

	/** Stateクラスインスタンス */
	State state;
	/** Cacheクラスインスタンス */
	Cache cache;
	/** オペレーションの名前 */
	String opr;
	/** オペレーションの対象となるパッケージ名 */
	String name;

    /**
     * 設定のチェックに成功していれば、オペレーションを実行する
     * 問題があるならシステムメッセージを表示する
     * @return オペレーションの結果     */
    int main(String[] args)
    {
		int ret;
		state = new State();
		state.init();
		if (!state.lock(LOCKFILE)) {
			ERR << """
klip can't get lock...
please check ~/.konoha/klip/klip.lock
"""
			return 1;
		}
		Logger.createLogger(state.klipdir + LOGFILE, true);
		ErrorLogger.createLogger(state.klipdir + ERRLOGFILE, true);
		if (state.isFailed()) {
			state.info();
			return exit_klip(EXIT_ERROR);
		}
		if (!checkArgs(args)) {
			klip_show_usage();
			return exit_klip(EXIT_NORMAL);
		}
		cache = new Cache(state.klipdir,KLIPURL,DATAFILE);
		cache.init();
		ret = start();
		return exit_klip(ret);
	}

	int exit_klip(int ret)
	{
		state.release(LOCKFILE);
		return ret;
	}

   	/**
	 * コマンドラインに入力された引数が正しいかチェックする
	 */
	boolean checkArgs(String[] args)
	{
		boolean ret = false;
		if (|args| >= 2 && |args| <= 3) {
			_opr = args[1];
			if (!checkOpr()) { ret = false; }
			else { ret = true; }
		}
		if (!ret) { return ret; }
		
		DictMap? ops = operations[_opr];
		if ((int)ops["has_arg"] > 0) {
			if (|args| == 3 ) { _name = args[2]; ret = true; }
			else { ret = false; }
		} else { ret = true; }
		return ret;
	}

	boolean checkOpr()
	{
		boolean ret = true;
		switch (_opr) {
		case "-i": _opr = "install"; break;
		case "-u": _opr = "update"; break;
		case "-r": _opr = "remove"; break;
		case "-l": _opr = "list"; break;
		case "-s": _opr = "search"; break;
		default :
			ret = (_opr in ["install","update","remove",
							"list","remove","list","clean",
							"installed","search","undo","env"]);
			break;
		}
		return ret;
	}

	/**
	 * オペレーションの開始
	 * @return オペレーションの結果
	 */
	int start()
	{
		int ret;
		boolean yet = false;
		switch (_opr) {
		case "env"      : ret = displayEnv(); break;
		case "installed": ret = showList(_cache.curdata); break;
		case "undo"     : ret = undoMain(); break;
		case "clean"    : ret = cleanMain(); break;
		case "remove"   : ret = packageOpration(); break;
		default : yet = true; break;
		}

		if (!yet) { return exit_operation(ret); }
		
		_cache.getklipdata();
		switch (_opr) {
		case "list"    : ret = showList(_cache.pkgdata); break;
		case "search"  : ret = searchMain(); break;
		case "install" : ret = packageOpration(); break;
		case "update"  : ret = packageOpration(); break;
		default : ret = EXIT_WARN; break;
		}

		return exit_operation(ret);
	}

	int exit_operation(int ret)
	{
		Logger logger = Logger.getLogger();
		Logger errlogger = ErrorLogger.getLogger();
		if ( ret == EXIT_SUCCESS ) {
			if (_opr in ["install","update"]) {
				logger << %(SUCCESS_INFO,_state.instdir);
			}
			logger << "KLIP : " + _opr + " terminated with success";
		} else if ( ret == EXIT_NORMAL ) {
			logger << "KLIP : " + _opr + " exit normaly";
		} else if (ret == EXIT_WARN ) {
			logger << "KLIP : exit with warning";
		}else if ( ret == EXIT_ERROR ) {
			errlogger << " " + _opr + " faild";
		}
		return ret;
	}

	/**
	 * パッケージの確認が必要なオペレーションを実行する
	 * @return オペレーションの結果
	 */
	int packageOpration()
	{
		int ret = EXIT_NORMAL;
		Logger logger = Logger.getLogger();
		Package cur;
		Package pkg;
		String cur_ver;
		String new_ver;
		boolean in_current = hasPackage(_name,_cache.curdata);
		boolean in_pkglist = hasPackage(_name,_cache.pkgdata);
		boolean in_pkgdir  = os.hasDir(_state.instdir + _name);

		if (in_pkglist) {
			pkg = getPackage(_name,_cache.pkgdata);
			new_ver = pkg.version;
		}

		if (in_current) {
			cur = getPackage(_name,_cache.curdata);
			cur_ver = cur.version;
		}

		if (_opr == "install") {
			if (!in_pkglist) {
				logger << %("INFO : package \"%s{0}\"is NOT FOUND",_name);
			}
			else if (in_current) {
				logger << "INFO : package is already installed";
			}
			else if (in_pkgdir) {
				logger << "WARN : klip can't touch this existed package";
				ret = EXIT_WARN;
			}
			else {
				ret = installMain(pkg);
			}
		}
		else if (_opr == "update") {
			if (!in_pkglist) {
				logger << %("INFO : package \"%s{0}\"is NOT FOUND",_name);
			}
			else if (!in_pkgdir) {
				logger << "INFO : package isn't installed";
			}
			else if (!in_current) {
				logger << "WARN : klip can't touch this existed package";
				ret = EXIT_WARN;
			}
			else if (cur_ver >= new_ver) {
				logger << "INFO : latest package is already installed";
			}
			else {
				ret = updateMain(pkg);
			}
		}
		else if (_opr == "remove") {
			if (!in_pkgdir) {
				logger << "INFO : package isn't installed";
			}
			else if (!in_current) {
				logger << "WARN : klip can't touch this existed package";
				ret = EXIT_WARN;
			}
			else {
				ret = removeMain(cur);
			}
		}
		
		return ret;
	}

	//common

	/**
	 * 特定パッケージのインスタンスを作成し、
	 * 指定したリストファイルからの情報をセットして返す
	 * @param pkgname パッケージ名
	 * @param filename リストファイル名
	 * @return 作成したパッケージインスタンス
	 */
	Package getPackage(String pkgname, String filename)
	{
		Package pac = new Package(state,pkgname);
		PackageList pl = ListFactory.getPackageList(filename);
		DictMap map = pl.getMember(pkgname);
		pac.setMember(map);
		return pac;
	}

	/**
	 * 指定したリストファイルに、パッケージが含まれているか確認する
	 * @param pkgname パッケージ名
	 * @param filename リストファイル名
	 * @return パッケージの有無
	 */
	boolean hasPackage(String pkgname,String filename)
	{
		Package pkg = getPackage(pkgname,filename);
		boolean retv = (pkg.version.getSize() != 0);
		boolean retu = (pkg.url.getSize() != 0);
		return (retv && retu);
	}

	//search & list

	/**
	 * リストファイルからパッケージの情報を検索する
	 * @param filename リストファイル名
	 * @return 検索の結果
	 */
	int searchMain()
	{
		Logger logger = Logger.getLogger();
		if (hasPackage(_name,_cache.pkgdata)) {
			logger << "INFO : package FOUND!";
			Package pkg = getPackage(_name,_cache.pkgdata);
			OUT << %("%s{0}  (%s{1}):", _name, pkg.version) << EOL;
		} else {
			logger << %("INFO : package \"%s{0}\" NOT FOUND", _name);
		}
		return EXIT_SUCCESS;
	}

	/**
	 * リストファイルの全パッケージ情報の一覧を表示する
	 * @param filename リストファイル名
	 * @return リスト表示の結果
	 */
	int showList(String filename)
	{
		if ( !os.hasFile(filename) ) { return EXIT_ERROR; }
		Logger logger = Logger.getLogger();
		PackageList plist = ListFactory.getPackageList(filename);
		String[] list = plist.getPackageNameList();
		foreach (String nm from list) {
			if (!hasPackage(nm,filename) ) { continue; }
			Package pkg = getPackage(nm,filename);
			OUT << %("%s{0}\t(%s{1}):",nm,pkg.version) << EOL;
		}
		logger << "KLIP : theere are [" + |list| + "] packages";
		return EXIT_SUCCESS;
	}

	
	//system operations

	int displayEnv()
	{
		Logger logger = Logger.getLogger();
		logger << "KLIP : show system info & Klip's environ variables\n";

		//----- display Konoha info -----//
		logger << %("""
[current_working_system]
  Konoha   : %s{0}
  platform : %s{1}
  Klip     : Klip %d{2}.%d{3}.%d{4} (%s{5})
""",%k(os),$konoha.platform,VERSION,MAJOR,MINOR,LASTUPDATE);

		//----- display klip dirpath -----//
		logger << %("""
[state]
  chk_root : %s{0}
  instdir  : %s{1}
  klipdir  : %s{2}
  tmpdir   : %s{3}
""",_state.chk_root,_state.instdir,_state.klipdir,_State.tmpdir);

		//----- display klip cachefiles -----//
		logger << %("""
[cache]
  pkgdata  : %s{0}
  dataurl  : %s{1}
  curdata  : %s{2}
  prevdata : %s{3}
  latest   : %s{4}
""",_cache.pkgdata,_cache.dataurl,_cache.curdata,_cache.prevdata,_cache.latest);
		
		return EXIT_SUCCESS;
	}


	/**
	 * システムファイルを初期の状態に初期化する
	 * @return 初期化の結果を返す
	 */
	int cleanMain()
	{
		cache.removeAll();
		return EXIT_SUCCESS;
	}


	//package operations

	/**
	 * パッケージをインストールの為の依存性の確認や、
	 * 実行に伴ったシステムファイル操作等を行う
	 * @param pkg インストールするパッケージ
	 * @return インストールの結果
	 */
	int installMain(Package pkg)
	{
		int ret;
		cache.mkBufCurData();
		String str =  pkg.name + "=" + pkg.version;
		cache.updateLatest("Last-Installed-Package:",str);
		if ( pkg.depend.getSize() == 0 ) {
			ret = pkg.install();
			if ( ret == EXIT_SUCCESS ) {
				cache.modCurData([pkg.name,pkg.version,pkg.url],"a");
			}
		} else {
			ret = dinstall(pkg);
		}
		if ( ret == EXIT_SUCCESS ) { cache.cpBufCur2Prev();	}
		cache.rmBufCurData();
		return ret;
	}
	
	/**
	 * パッケージ更新に伴ったシステムファイル操作等を行う
	 * @param pkg 更新するパッケージ
	 * @return 更新の結果
	 */
	int updateMain(Package pkg)
	{
		int ret;
		cache.mkBufCurData();
		String str =  pkg.name + "=" + pkg.version;
		cache.updateLatest("Last-Installed-Package:",str);
   		ret = pkg.update();
		if ( ret == EXIT_SUCCESS ) {
			cache.modCurData([pkg.name,pkg.version,pkg.url],"r");
			cache.cpBufCur2Prev();
		}
		cache.rmBufCurData();
		return ret;
	}
	
	/**
	 * パッケージ削除に伴ったシステムファイル操作等を行う
	 * @param pkg 削除するパッケージ
	 * @return 削除の結果
	 */
	int removeMain(Package pkg)
	{
		int ret;
		cache.mkBufCurData();
		ret = pkg.remove();
		if ( ret == EXIT_SUCCESS ) {
			cache.modCurData([pkg.name,pkg.version,pkg.url],"d");
			cache.cpBufCur2Prev();
		}
		cache.rmBufCurData();
		return ret;
	}
	
	/**
	 * 依存性のあるパッケージも含めたインストールを行う
	 * @param pkg インストールの主目的となるパッケージ
	 * @return インストールしたパッケージの数
	 */
	int dinstall(Package pkg)
	{
		String[] list = getDependList(pkg);
		int num = |list|;
		int ret = 0;
		while (num > 0) {
			String[] strs = list[num - 1].split("=",0);
			String sname = strs[0];
			String sver  = strs.pop();
			Package pkg = getPackage(sname,cache.pkgdata);
			num--;
			if (pkg.version < sver) { continue; }
			if (pkg.install() == EXIT_SUCCESS) {
				cache.modCurData([sname,pkg.version,pkg.url],"a");
				ret++;
			}
		}
		return ret;
	}
	
	/**
	 * パッケージが依存するパッケージ情報を文字列配列に出力する
	 * @param pkg 基準となるパッケージ
	 * @return 依存するパッケージ情報の文字列配列
	 */
	String[] getDependList(Package pkg)
	{
		Logger logger = Logger.getLogger();
		logger << "KLIP : resolve dependency...";
		String[] list = [pkg.name + "=" + pkg.version];
		resolveDepend(pkg,list);
		return list;
	}

	/**
	 * パッケージの依存するパッケージ情報のリストを、
	 * 子となるパッケージの依存性も含めて再帰的に取得する
	 * @param cur 現在の基準となるパッケージ
	 * @param list 情報を追加していくリスト
	 */
	void resolveDepend(Package cur, String[] list)
	{
		Logger errlogger = ErrorLogger.getLogger();
		foreach (String txt from cur.depend) {
			if (!searchList(txt,list,false)) { continue; }
			String sname = txt.split("=",0)[0];
			String sver = txt.split("=",0).pop();
			if(!hasPackage(sname,_cache.pkgdata) ) {
				errlogger << "WARN : resolveDepend : PKG NOT FOUND" + sname;
				continue;
			}
			Package pkg = getPackage(sname,_cache.pkgdata);
			String gver = pkg.version;
			if (sver > gver) {
				errlogger << "WARN : not found required version : " + sname;
				errlogger << %("WARN : req.[%s{0}] < get[%s{1}]",sver,gver);
				continue;
			}
			searchList(sname + "=" + gver,list,true);
			resolveDepend(pkg,list);
		}
	}
	
	/**
	 * 依存パッケージのリストから、指定のパッケージが既に存在するかを検索する
	 * @param pkgname パッケージ名
	 * @param list リストとなる文字列配列
	 * @param update 発見した場合に更新するか否か
	 * @return 検索の結果
	 */
	boolean searchList(String pkgname,String[] list,boolean update)
	{
		boolean ret;
		foreach (String txt from list) {
			String[] pkgnames = pkgname.split("=",0);
			String[] listnames = txt.split("=",0);
			if (pkgnames[0] == listnames[0]) {
				if (pkgnames.pop() <= listnames.pop()) { ret = false; }
				else if (update) {
					list[list.indexOf(txt)] = pkgname;
					ret = true;
				}
			}
		}
		if(update && !ret) { list << pkgname; ret = true; }
		return ret;
	}

	//undo

	/**
	 * アンドゥを行う
	 * @return アンドゥの結果を返す
	 */
	int undoMain()
	{
		int ret;
		Logger logger = Logger.getLogger();
		ret = undo("uninstall");
		ret = undo("reinstall");
		if (ret == EXIT_SUCCESS) { 
			logger << %(SUCCESS_INFO,_state.instdir);
		}
		return ret;
	}

	/*
	 * アンドゥの再インストール、削除の処理を行う
	 * @param unopr 再インストールまたは削除
	 * @return 結果
	 */
	int undo (String unopr)
	{
		int ret;
		Logger errlogger = ErrorLogger.getLogger();
		cache.mkBufCurData();
		if (unopr == "uninstall") {
			String[] list = diffText(_cache.curdata, _cache.prevdata);
			ret = do_undo("d",_cache.curdata,list);
		} else {
			String[] list = diffText(_cache.prevdata, _cache.curdata);
			ret = do_undo("a",_cache.prevdata,list);
		}
		if (ret == EXIT_ERROR) {
			errlogger << "!ERR : undo failed to " + unopr;
			_cache.rmBufCurData();
			return EXIT_ERROR;
		}
		_cache.cpBufCur2Prev();
		_cache.rmBufCurData();
		return ret;
	}

	/**
	 * リストに含まれるパッケージのインストール、または削除を行う
	 * @param mode インストールまたは削除
	 * @param file パッケージをチェックするリストファイル
	 * @param list パッケージリスト
	 * @return アンドゥの結果
	 */
	int do_undo(String mode,String file,String[] list)
	{
		int ret;
		if (|list| == 0 ) { return EXIT_NORMAL; }
		int done = 0;
		foreach( String txt from list ) {
			String setnm = txt.split('"name":"',0).pop();
			setnm = setnm.split('"',0)[0];
			if ( !hasPackage(setnm,file) ) { continue; }
			Package pkg = getPackage(setnm,file);
			if (mode == "a") { ret = pkg.install(); }
			else if (mode == "d") { ret = pkg.remove(); }
			if ( ret == EXIT_SUCCESS ) {
				cache.modCurData([pkg.name,pkg.version,pkg.url],mode);
				done += 1;
			}
		}
		if (done == |list|) { ret = EXIT_SUCCESS; }
		else if (done == 0) { ret = EXIT_ERROR; }
		else { ret = EXIT_WARN; }
		return ret;
	}

}

int main(String[] args)
{
	int ret;
	Klip klip = new Klip();
	ret = klip.main(args);
	return ret;
}
