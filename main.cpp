#include <clickhouse/client.h>
#include <iostream>
#include <string>
#include <vector>

using namespace clickhouse;
using namespace std;

std::vector<std::string> column_name(2, "");
std::vector<std::string> column_type(2, "");

int main() {
  /// Initialize client connection.
  Client client(ClientOptions().SetHost("localhost"));
  client.Execute("CREATE TABLE IF NOT EXISTS default.fuck (id UInt64, name String) ENGINE = Memory");

  /// Insert some values.
  {
    Block block;

    auto id = std::make_shared<ColumnUInt64>();
    id->Append(1);
    id->Append(7);

    auto name = std::make_shared<ColumnString>();
    name->Append("one");
    name->Append("seven");

    block.AppendColumn("id"  , id);
    block.AppendColumn("name", name);

    client.Insert("default.fuck", block);
  }

  /// Select values inserted in the previous step.
  client.Select("SELECT id, name FROM default.fuck", [] (const Block& block)
                {
                  for (size_t i = 0; i < block.GetRowCount(); ++i) {
                    std::cout << block[0]->As<ColumnUInt64>()->At(i) << " "
                              << block[1]->As<ColumnString>()->At(i) << "\n";
                  }
                }
  );

  client.Select("SELECT column_name, data_type FROM INFORMATION_SCHEMA.COLUMNS "
                "where table_name = \'fuck\'", [] (const Block& block)
                {
                  for (size_t i = 0; i < block.GetRowCount(); ++i) {

                    column_name[i] = block[0]->As<ColumnString>()->At(i);
                    column_type[i] = block[1]->As<ColumnString>()->At(i);
                  }
                }
  );
  /*
  std::cout << block.GetRowCount() << std::endl;
  for (size_t i = 0; i < block.GetRowCount(); ++i) {
    column_name[i] = block[0]->As<ColumnString>()->At(i);
    column_type[i] = block[1]->As<ColumnString>()->At(i);
  }
   */

  for (int i = 0; i < 2; ++i) {
    std::cout << column_name[i] << ", " << column_type[i] << std::endl;
  }


  /// Delete table.
  //client.Execute("DROP TABLE default.numbers");

  return 0;
}



