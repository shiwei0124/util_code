#define LOAD_FILE(doc, xml) \
	rapidxml::file<> fdoc(xml);\
	doc.parse<rapidxml::parse_comment_nodes | rapidxml::parse_doctype_node>(fdoc.data());

#define FIND_DOC_ELEM(doc, node, elem) \
	{\
	node = doc.first_node(elem);\
	if (!node)\
		{\
		return HPR_FALSE;\
		}\
	}

#define FIND_ELEM(childnode, parentnode, elem) \
	{\
	childnode = parentnode->first_node(elem);\
	if (!childnode)\
		{\
		return HPR_FALSE;\
		}\
	}

#define FIND_ATTRIBUTE(attribute, node, elem) \
	{\
	attribute = node->first_attribute(elem);\
	if (!attribute)\
		{\
		return HPR_FALSE;\
		}\
	}


/*
<?xml version="1.0" encoding="utf-8"?>
<devanalysis>
	<server>										<!-- local server info -->
		<ip>172.7.3.53</ip>							<!-- local ip -->
		<port>8879</port>				<!-- listen port -->
	</server>
	<database>
		<dbip>10.1.17.12</dbip>								<!-- database ip -->
		<dbport>3306</dbport>							<!-- database port -->
		<dbname>shipin7</dbname>							<!-- database name -->
		<dbuser>hikapp</dbuser>							<!-- database user -->
		<dbpwd>hik88075998</dbpwd>								<!-- database pwd -->
	</database>
	<options>
		<heartbeatspace>900</heartbeatspace>			<!-- heartbeat space of unbinding dev, unit: sec -->
	</options>
</devanalysis>
*/
HPR_BOOL CConfiger::LoadConfiger()
{
	HPR_BOOL bRet = HPR_FALSE;
	rapidxml::xml_document<> doc;

	try
	{
		LOAD_FILE(doc, "devanalysis.xml");

		rapidxml::xml_node<> *node;
		rapidxml::xml_node<> *nodechild;

		FIND_DOC_ELEM(doc, node, "devanalysis");
		
		FIND_ELEM(nodechild, node, "server");
		node = nodechild;

		FIND_ELEM(nodechild, node, "ip");
		SetLocalIP(nodechild->value());

		FIND_ELEM(nodechild, node, "port");
		SetListenPort(atoi(nodechild->value()));

		node = node->parent();

		FIND_ELEM(nodechild, node, "database");
		node = nodechild;

		FIND_ELEM(nodechild, node, "dbip");
		SetSQLIP(nodechild->value());
		
		FIND_ELEM(nodechild, node, "dbport");
		SetSQLPort(atoi(nodechild->value()));

		FIND_ELEM(nodechild, node, "dbname");
		SetDBName(nodechild->value());

		FIND_ELEM(nodechild, node, "dbuser");
		SetDBUserName(nodechild->value());

		FIND_ELEM(nodechild, node, "dbpwd");
		SetDBPassword(nodechild->value());

		node = node->parent();

		FIND_ELEM(nodechild, node, "options");
		node = nodechild;

		FIND_ELEM(nodechild, node, "heartbeatspace");
		SetHeartSpace(nodechild->value());
		bRet = HPR_TRUE;
	}
	catch (...)
	{
		bRet = HPR_FALSE;
	}
	return bRet;
}