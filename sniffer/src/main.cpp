#include <iostream>
#include <vector>

#include "PcapLiveDeviceList.h"
#include "SystemUtils.h"
#include "SQLiteCpp/SQLiteCpp.h"

#include "sniffer.hpp"

/**
* main method of the application
*/
int main(int argc, char* argv[])
{

	std::vector<pcpp::PcapLiveDevice*> allDevs = pcpp::PcapLiveDeviceList::getInstance().getPcapLiveDevicesList();
	std::vector<Sniffer> sniffers;
	std::for_each(allDevs.begin(), allDevs.end(), [&sniffers](pcpp::PcapLiveDevice* dev) {
		if(dev->getMtu() != 0)
			sniffers.push_back(Sniffer(dev));
	});

   


	std::for_each(sniffers.begin(), sniffers.end(), [&sniffers](Sniffer &sniffer) {
		try
		{
			sniffer.printInfo();
			sniffer.startCapture();
		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << '\n';
			std::remove(sniffers.begin(), sniffers.end(), sniffer);
		}
	});
	// 定时捕获测试
	pcpp::multiPlatformSleep(5);
	std::for_each(sniffers.begin(), sniffers.end(), [](Sniffer &sniffer) {
		sniffer.stopCapture();
        sniffer.printLog();

	});


	// std::cout << "SQlite3 version " << SQLite::VERSION << " (" << SQLite::getLibVersion() << ")" << std::endl;
    // std::cout << "SQliteC++ version " << SQLITECPP_VERSION << std::endl;

    // ////////////////////////////////////////////////////////////////////////////
    // // Simple batch queries example :
    // try
    // {
    //     // Open a database file in create/write mode
    //     SQLite::Database    db("test.db3", SQLite::OPEN_READWRITE|SQLite::OPEN_CREATE);
    //     std::cout << "SQLite database file '" << db.getFilename().c_str() << "' opened successfully\n";

    //     // Create a new table with an explicit "id" column aliasing the underlying rowid
    //     db.exec("DROP TABLE IF EXISTS test");
    //     db.exec("CREATE TABLE test (id INTEGER PRIMARY KEY, value TEXT)");

    //     // first row
    //     int nb = db.exec("INSERT INTO test VALUES (NULL, \"test\")");
    //     std::cout << "INSERT INTO test VALUES (NULL, \"test\")\", returned " << nb << std::endl;

    //     // second row
    //     nb = db.exec("INSERT INTO test VALUES (NULL, \"second\")");
    //     std::cout << "INSERT INTO test VALUES (NULL, \"second\")\", returned " << nb << std::endl;

    //     // update the second row
    //     nb = db.exec("UPDATE test SET value=\"second-updated\" WHERE id='2'");
    //     std::cout << "UPDATE test SET value=\"second-updated\" WHERE id='2', returned " << nb << std::endl;

    //     // Check the results : expect two row of result
    //     SQLite::Statement   query(db, "SELECT * FROM test");
    //     std::cout << "SELECT * FROM test :\n";
    //     while (query.executeStep())
    //     {
    //         std::cout << "row (" << query.getColumn(0) << ", \"" << query.getColumn(1) << "\")\n";
    //     }

    //     db.exec("DROP TABLE test");
    // }
    // catch (std::exception& e)
    // {
    //     std::cout << "SQLite exception: " << e.what() << std::endl;
    //     return EXIT_FAILURE; // unexpected error : exit the example program
    // }
    // remove("test.db3");

    // std::cout << "everything ok, quitting\n";

	return 0;
}