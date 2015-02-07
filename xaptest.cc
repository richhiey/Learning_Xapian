#include <xapian.h>
#include <iostream>
#include <string.h>
#include <cstring>

using namespace std;



int main()
{
	std::string index_or_search;
	std::string dbpath;
	std::string search = "search";
	std::string index = "index";
	cout << "Type:\n index for indexing documents\n search for searching documents" << std::endl;
	cin >> index_or_search;
	cout << "Enter the dbpath" << std::endl;
	cin >> dbpath;
	
	try{
	
	//Indexing
	if(index_or_search == index)
	{
		
		//Opening a writeable database
		Xapian::WritableDatabase db(dbpath, Xapian::DB_CREATE_OR_OPEN);
	
		//Setting up term generator for indexing the docs
		Xapian::TermGenerator indexer;
		Xapian::Stem stemmer("english");
		indexer.set_stemmer(stemmer);
		
		std::string line;
		std::string docline;
		
		//Getting the data		
		while(true)
		{
			if(!cin.eof())
			{
				getline(cin,line);
				docline += line;
			}
		}
		
		//Creating a document, setting the data, adding it to the database
		if(!docline.empty())
		{
			Xapian::Document doc;
			doc.set_data(docline);
			indexer.set_document(doc);
			indexer.index_text(docline);
			db.add_document(doc);
		
			std::cout << "" << indexer.get_description() << std::endl;
		}
		
		//Commit the changes made to the database		
		db.commit();
	}
	
	//Searching
	else if(index_or_search == search)
	{
		//The query to be processed		
		std::string query_input;
		std::cout << "Enter the query to be searched for" << std::endl;
		std::cin >> query_input;

		//Create a read only database and pass it to the Enquire object		
		Xapian::Database db(dbpath);
		Xapian::Enquire enquire(db);
		
		std::string query = query_input;
			
		//Set the QueryParser functions		
		Xapian::QueryParser qp;
		Xapian::Stem stemmer("english");
		qp.set_stemmer(stemmer);
		qp.set_database(db);
		qp.set_stemming_strategy(Xapian::QueryParser::STEM_SOME);
		
		//Create the query by parsing it to the query object
		Xapian::Query q = qp.parse_query(query);
		
		std::cout<<"Parsed query is"<<q.get_description()<<std::endl;
		enquire.set_query(q);
		
		//Get the MSet	
		Xapian::MSet match = enquire.get_mset(0,10);
	
		
		std::cout<< match.get_matches_estimated() << "results found" << endl;
		std::cout<<"results 1-"<<match.size() << endl;
		
		//Iterating through all docs
		for(Xapian::MSetIterator i = match.begin(); i!=match.end(); i++)
		{
			std::cout << "Rank:" << i.get_rank() <<"\t Doc:" << i.get_document().get_data() << "Percent:" << i.get_percent() << std::endl;
		}
	}
		
	else
		std::cout << "Invalid input. Try again" << std::endl;
	
									
	}catch(const Xapian::Error &error)										
	{
		std::cout << "Exception:" << error.get_msg() << std::endl;
	}
}

		
