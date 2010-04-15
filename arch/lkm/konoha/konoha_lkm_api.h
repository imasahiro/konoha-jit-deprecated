#ifdef KONOHA_ON_LKM
#define __tobytes(s)      knh_String_tobytes(s)
EXPORT_SYMBOL(new_Array);
EXPORT_SYMBOL(new_IArray);
EXPORT_SYMBOL(new_FArray);
EXPORT_SYMBOL(knh_Array_add);
EXPORT_SYMBOL(new_Bytes);
EXPORT_SYMBOL(knh_Bytes_tobytes);
EXPORT_SYMBOL(knh_Bytes_clear);
EXPORT_SYMBOL(knh_Bytes_ensureSize);
EXPORT_SYMBOL(knh_Bytes_putc);
EXPORT_SYMBOL(knh_Bytes_write);
EXPORT_SYMBOL(new_bytes);
EXPORT_SYMBOL(new_bytes__2);
EXPORT_SYMBOL(knh_bytes_indexOf);
EXPORT_SYMBOL(knh_bytes_strcmp);
EXPORT_SYMBOL(knh_bytes_strcasecmp);
EXPORT_SYMBOL(knh_bytes_equals);
EXPORT_SYMBOL(knh_bytes_equalsIgnoreCase);
EXPORT_SYMBOL(knh_bytes_startsWith);
EXPORT_SYMBOL(knh_bytes_endsWith);
EXPORT_SYMBOL(knh_bytes_matchWildCard);
EXPORT_SYMBOL(knh_bytes_index);
EXPORT_SYMBOL(knh_bytes_rindex);
EXPORT_SYMBOL(knh_bytes_mod);
EXPORT_SYMBOL(knh_bytes_rmod);
EXPORT_SYMBOL(knh_bytes_first);
EXPORT_SYMBOL(knh_bytes_last);
EXPORT_SYMBOL(knh_bytes_skipscheme);
EXPORT_SYMBOL(knh_format_bytes);
EXPORT_SYMBOL(knh_format_join2);
EXPORT_SYMBOL(knh_bytes_toint);
EXPORT_SYMBOL(knh_bytes_tofloat);
EXPORT_SYMBOL(knh_bytes_toint64);
EXPORT_SYMBOL(knh_bytes_parsecid);
EXPORT_SYMBOL(knh_bytes_parseURLscheme);
EXPORT_SYMBOL(knh_bytes_parseURLuname);
EXPORT_SYMBOL(knh_bytes_parseURL);
EXPORT_SYMBOL(knh_bytes_substringURLpath);
EXPORT_SYMBOL(knh_bytes_parseURLpath);
EXPORT_SYMBOL(knh_bytes_parseURLhost);
EXPORT_SYMBOL(knh_bytes_parseURLport);
EXPORT_SYMBOL(new_String__fbcnv);
EXPORT_SYMBOL(new_Enum);
EXPORT_SYMBOL(new_Unit);
EXPORT_SYMBOL(new_Vocab);
EXPORT_SYMBOL(knh_loadURNAliasData);
EXPORT_SYMBOL(knh_loadClassSpecFuncData);
EXPORT_SYMBOL(knh_putsfp);
EXPORT_SYMBOL(knh_Closure_invokesfp);
EXPORT_SYMBOL(knh_Closure_invokef);
EXPORT_SYMBOL(knh_Closure_invoke);
EXPORT_SYMBOL(new_Exception);
EXPORT_SYMBOL(knh_cwb_newException);
EXPORT_SYMBOL(new_Exception__T);
EXPORT_SYMBOL(knh_cwb_perrno);
EXPORT_SYMBOL(knh_throw_Unsupported);
EXPORT_SYMBOL(knh_throw_OutOfIndex);
EXPORT_SYMBOL(new_InputStream__io);
EXPORT_SYMBOL(new_InputStream__FILE);
EXPORT_SYMBOL(new_FileInputStream);
EXPORT_SYMBOL(new_BytesInputStream);
EXPORT_SYMBOL(new_StringInputStream);
EXPORT_SYMBOL(knh_Iterator_close);
EXPORT_SYMBOL(new_Iterator);
EXPORT_SYMBOL(new_GlueIterator);
EXPORT_SYMBOL(new_ArrayIterator);
EXPORT_SYMBOL(new_Mapper);
EXPORT_SYMBOL(knh_addMapper);
EXPORT_SYMBOL(knh_findcid);
EXPORT_SYMBOL(new_Int);
EXPORT_SYMBOL(new_IntX__fast);
EXPORT_SYMBOL(new_IntX);
EXPORT_SYMBOL(new_Float);
EXPORT_SYMBOL(new_FloatX__fast);
EXPORT_SYMBOL(new_FloatX);
EXPORT_SYMBOL(knh_addAffineMapper);
EXPORT_SYMBOL(knh_Glue_init);
EXPORT_SYMBOL(new_Glue);
EXPORT_SYMBOL(new_OutputStream__io);
EXPORT_SYMBOL(new_OutputStream__FILE);
EXPORT_SYMBOL(new_FileOutputStream);
EXPORT_SYMBOL(new_BytesOutputStream);
EXPORT_SYMBOL(knh_ResultSet_initColumn);
EXPORT_SYMBOL(knh_ResultSet_setName);
EXPORT_SYMBOL(knh_ResultSet_initData);
EXPORT_SYMBOL(knh_ResultSet_setInt);
EXPORT_SYMBOL(knh_ResultSet_setFloat);
EXPORT_SYMBOL(knh_ResultSet_setText);
EXPORT_SYMBOL(knh_ResultSet_setBlob);
EXPORT_SYMBOL(knh_ResultSet_setNULL);
EXPORT_SYMBOL(new_String);
EXPORT_SYMBOL(knh_String_tobytes);
EXPORT_SYMBOL(knh_String_text);
EXPORT_SYMBOL(knh_addParserDriver);
EXPORT_SYMBOL(knh_setSystemPropertyText);
EXPORT_SYMBOL(new_Pair);
EXPORT_SYMBOL(new_Tuple);
EXPORT_SYMBOL(new_Range);
EXPORT_SYMBOL(knh_putc);
EXPORT_SYMBOL(knh_write);
EXPORT_SYMBOL(knh_flush);
EXPORT_SYMBOL(knh_print);
EXPORT_SYMBOL(knh_println);
EXPORT_SYMBOL(knh_write_EOL);
EXPORT_SYMBOL(knh_write_TAB);
EXPORT_SYMBOL(knh_write_BOL);
EXPORT_SYMBOL(knh_format);
EXPORT_SYMBOL(knh_printf);
EXPORT_SYMBOL(knh_says);
EXPORT_SYMBOL(knh_fopen);
EXPORT_SYMBOL(knh_fgetc);
EXPORT_SYMBOL(knh_fread);
EXPORT_SYMBOL(knh_fwrite);
EXPORT_SYMBOL(knh_fflush);
EXPORT_SYMBOL(knh_fclose);
EXPORT_SYMBOL(knh_addIODriver);
EXPORT_SYMBOL(knh_addRegexDriver);
EXPORT_SYMBOL(new_Socket);
EXPORT_SYMBOL(knh_socket_open);
EXPORT_SYMBOL(knh_socket_send);
EXPORT_SYMBOL(knh_socket_recv);
EXPORT_SYMBOL(knh_socket_close);
EXPORT_SYMBOL(knh_addDBDriver);
EXPORT_SYMBOL(konoha_SETv);
EXPORT_SYMBOL(konoha_FINALv);
EXPORT_SYMBOL(knh_beginContext);
EXPORT_SYMBOL(knh_endContext);
EXPORT_SYMBOL(knh_getCurrentContext);
EXPORT_SYMBOL(konoha_debugLevel);
EXPORT_SYMBOL(konoha_isSystemDump2);
EXPORT_SYMBOL(knh_isToInteractiveMode);
EXPORT_SYMBOL(konoha_parseopt);
EXPORT_SYMBOL(konoha_hasRuntimeError);
EXPORT_SYMBOL(konoha_getRuntimeError);
EXPORT_SYMBOL(konoha_setOutputStreamBuffer);
EXPORT_SYMBOL(konoha_getStdOutBufferText);
EXPORT_SYMBOL(konoha_getStdErrBufferText);
EXPORT_SYMBOL(konoha_evalScript);
EXPORT_SYMBOL(konoha_loadScript);
EXPORT_SYMBOL(konoha_runMain);
EXPORT_SYMBOL(konoha_hasScriptFunc);
EXPORT_SYMBOL(konoha_invokeScriptFunc);
EXPORT_SYMBOL(konoha_invokeIntFunc);
EXPORT_SYMBOL(konoha_invokeFloatFunc);
EXPORT_SYMBOL(konoha_invokeBooleanFunc);
EXPORT_SYMBOL(konoha_invokeStringFunc);
EXPORT_SYMBOL(konoha_setMethodFunc);
EXPORT_SYMBOL(konoha_shell);
EXPORT_SYMBOL(knh_stack_pmsg);
EXPORT_SYMBOL(knh_stack_p);
EXPORT_SYMBOL(knh_safefile);
EXPORT_SYMBOL(new_Object_boxing);
EXPORT_SYMBOL(knh_getPassword);
EXPORT_SYMBOL(knh_stack_checkSecurityManager);
EXPORT_SYMBOL(knh_stack_toArray);
EXPORT_SYMBOL(knh_stack_boxing);
EXPORT_SYMBOL(knh_stack_unboxing);
EXPORT_SYMBOL(knh_stack_w);
EXPORT_SYMBOL(knh_stack_typecheck);
EXPORT_SYMBOL(knh_stack_throw);
EXPORT_SYMBOL(knh_loadIntConstData);
EXPORT_SYMBOL(knh_loadFloatConstData);
EXPORT_SYMBOL(knh_loadStringConstData);
EXPORT_SYMBOL(knh_getSystemConst);
EXPORT_SYMBOL(knh_getmn);
EXPORT_SYMBOL(konoha_open);
EXPORT_SYMBOL(konoha_close);
EXPORT_SYMBOL(knh_class_instanceof);
EXPORT_SYMBOL(knh_Object_opTypeOf);
EXPORT_SYMBOL(Script__k);
EXPORT_SYMBOL(Object_opInstanceof);
EXPORT_SYMBOL(Any_opAdd);
EXPORT_SYMBOL(Any_opNeg);
EXPORT_SYMBOL(Socket_new);
EXPORT_SYMBOL(Socket_getInputStream);
EXPORT_SYMBOL(Socket_getOutputStream);
EXPORT_SYMBOL(Socket_close);
EXPORT_SYMBOL(Socket_isClosed);
EXPORT_SYMBOL(DictMap_keys);
EXPORT_SYMBOL(DictMap_values);
EXPORT_SYMBOL(knh_fmethod_abstract);
EXPORT_SYMBOL(knh_fmethod_formatter);
EXPORT_SYMBOL(knh_KLRCode_exec);
EXPORT_SYMBOL(Script_changeChannel);
EXPORT_SYMBOL(Script_hook);
EXPORT_SYMBOL(Script_getLines);
EXPORT_SYMBOL(setjmp);
#endif