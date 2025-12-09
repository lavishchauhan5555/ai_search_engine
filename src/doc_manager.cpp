#include "doc_manager.h"
#include <fstream>

void DocManager::saveDocMapping(int docId, const std::string& url,
                                const std::string& outputPath)
{
    std::ofstream out(outputPath, std::ios::app);
    out << docId << " " << url << "\n";
}
