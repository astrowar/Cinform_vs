#pragma once

#ifndef  PreCodeGenerate_HPP
#define  PreCodeGenerate_HPP

#include "cinform.hpp" 
#include "pmatch_baseClass.hpp"

#include "hsymbol.hpp"
 
#include "createCodeTree.hpp"


namespace CInform
{
	  
	namespace CodeParser
	{
		bool isSameNoum(std::string a, std::string b);

		 

		class ParserStoreSymbolList
		{
			std::list<std::pair<std::string, std::string>> locals;
			std::list<Symbol*> symbols;
		public:
			void addSymbol(Symbol* s); 
			std::list<Symbol*> ListSymbols();

			bool isSymbol(std::string name);
			bool isValue(std::string name);
			bool isKind(std::string name);
			bool isInstance(std::string name);		 
			SReference getReference(std::string name);
			SReference getKindOf( std::string name );
			SReference getKindOf( SReference aref );

			bool setLocalVar( std::string name, std::string value );
			bool getLocalVar( std::string name, std::string &value );
		};


		class ParserStore
		{
			int temp_id = 100;
			GrammarContext *_grammar;
			std::list<ParserStoreSymbolList>	symbol_stack;

			 
		public:
			ParserStore(GrammarContext *gtx);
			GrammarContext * grammar();
			CodeTree::SourceControl *codeTree;

			void push_state();
			std::string next_temp();

			SReference getReference(string name); //obtem o nome interno do objeto

			SReference getVerbReference( string name );

			SReference getKindOf( SReference ref );

			bool isValue(std::string name);			
			bool isKind(std::string name);
			bool isImplicityVar( std::string name );
			bool isInstance(std::string name);
			bool isArticle( std::string name );

			bool isSymbol(std::string name);
			bool addVerb( string name );


			bool addKind( string name, SReference kindbase );

			bool addInstance(std::string name, SReference kindbase);
			bool addProperty( SReference objectReceiver, std::string prop_name, SReference prop_kind );

			bool addGlobalVariable( std::string name, SReference kindbase );

			bool add( Symbol *s );

			bool setLocalVar( string var ,string value);
			bool getLocalVar( string var, string &value );

			std::string mangleg( std::string x );
			//std::string mangleg( std::string x, std::string sub );
			//std::string mangleg( std::string x, std::string sub1, std::string sub2 );

			std::string mangleg( std::string x, SReference sub );
			std::string mangleg( std::string x, SReference sub1, SReference sub2 );


			bool isComposeType( std::string name );
			std::list<std::string> demangleg( std::string x );

			bool isNoumReserved( std::string x );
			bool isPreposition( std::string x);
			bool isVerb( std::string x );
			bool isPronoum( std::string x );


			bool isPropertyOf( std::string prop_arroba_obj, SReference  &obj, SReference &internal_prop_name );
			 
		};

		class PreCodeGenerate
		{

		public:
			PreCodeGenerate* tail = nullptr;
			virtual bool isError() = 0;
			virtual std::string repr() = 0;

			PreCodeGenerate* add(PreCodeGenerate* atail);
		};

		class PreCodeGenerateError : public PreCodeGenerate
		{
		public:
			virtual bool isError() { return true; }
			std::string errorMessage;
			PreCodeGenerateError(std::string _errorMessage);
			virtual std::string repr();
		};


	 
		//esse codigo depende de um simbolo que ainda nao esta definido
		class PreCodeGenerateDependency : public PreCodeGenerate
		{
		public:
			virtual bool isError() { return false; }
			std::string requiredSymbol;
			PreCodeGenerateDependency(std::string requiredSymbol);
			virtual std::string repr();
		};

		class PreCodeGenerateEmpty : public PreCodeGenerate
		{
		public:
			virtual bool isError() { return false; } 
			PreCodeGenerateEmpty( );
			virtual std::string repr();
		};

	 
	 
		class PreCodeGenerateSession : public PreCodeGenerate
		{
		public:
			string session;
			PreCodeGenerate *inner;
			virtual std::string repr();
			virtual bool isError() { return false; }
			PreCodeGenerateSession(string _name);
		};

		class PreCodeGenerateBlock : public PreCodeGenerate
		{
		public:
			
			PreCodeGenerate *header;
			PreCodeGenerate *inner;
			virtual std::string repr();;
			virtual bool isError() { return false; }
			PreCodeGenerateBlock(PreCodeGenerate *_header); 
		};


		


		class PreCodeGenerateIL : public PreCodeGenerate
		{
			std::string verb, noum, x, y,z;
		public:
			virtual bool isError() { return false; }
			PreCodeGenerateIL();
			PreCodeGenerateIL(std::string _verb, std::string _noum, std::string _x, std::string _y, std::string _z);
			PreCodeGenerateIL(std::string _verb, std::string _noum, std::string _x, std::string _y);
			PreCodeGenerateIL(std::string _verb, std::string _noum, std::string _x);
		 
			virtual std::string repr();
		};

		PreCodeGenerateError* createPreCodeGenerateError( std::string _errorMessage );
		PreCodeGenerateEmpty * createPreCodeGenerateEmpty();

		PreCodeGenerateIL* createPreCodeGenerateIL( std::string _verb, std::string _noum, std::string _x, std::string _y, std::string _z );
		PreCodeGenerateIL* createPreCodeGenerateIL( std::string _verb, std::string _noum, std::string _x, std::string _y  );
		PreCodeGenerateIL* createPreCodeGenerateIL( std::string _verb, std::string _noum, std::string _x  ); 

		PreCodeGenerateSession* createPreCodeGenerateSession( string _name );
		PreCodeGenerateBlock* createPreCodeGenerateBlock( PreCodeGenerate *_header );

		PreCodeGenerateDependency* createPreCodeGenerateDependency( std::string requiredSymbol );

		class GenerateEntry
		{
		public:
			PreCodeGenerate*  IL;
			string entryName;

			GenerateEntry( string _entryName, PreCodeGenerate*  _IL ) : entryName( _entryName ), IL( _IL ) {}
		};

		string removeArticle( ParserStore *pstore, string compound );
		string  isCompound( ParserStore *pstore, string kindname, PreCodeGenerate** error );

		list<string> get_list_or( ParserStore *pstore, string slist );
		list<string> get_list_and( ParserStore *pstore, string slist );

		//GenerateEntry codeGenerateArticle( ParserStore *pstore, PreCodeGenerate* prev_generate, Match::TBlockGroupItemNoum vx );
		//GenerateEntry codeGenerateArticle( ParserStore *pstore, PreCodeGenerate* prev_generate, string compound );
		PreCodeGenerate* codeGenerate(ParserStore *pstore, PreCodeGenerate* prev_generate, std::string entryName, Match::TBlockGroupItemNoum vx, Match::TBlockGroupItemNoum vy, Match::TBlockGroupItemNoum vz);
	}
}

#endif