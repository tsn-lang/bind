#include "Common.h"

TEST_CASE("Test ISymbol", "[bind]") {
    Registry::Reset();

    DataType* sym0 = type<i32>("test");
    Namespace sym1 = Namespace("test");
    Function sym2 = Function("test", Registry::Signature<i32>(), nullptr);
    ValuePointer sym3 = ValuePointer("test", sym0, nullptr, nullptr);
    DataType* sym4 = sym1.type<u32>("test");
    Namespace sym5 = Namespace(&sym1, "test");
    Function sym6 = Function("test", Registry::Signature<i32>(), &sym5);
    ValuePointer sym7 = ValuePointer("test", sym0, nullptr, &sym1);

    REQUIRE(sym0->getSymbolType() == SymbolType::DataType);
    REQUIRE(sym1.getSymbolType() == SymbolType::Namespace);
    REQUIRE(sym2.getSymbolType() == SymbolType::Function);
    REQUIRE(sym3.getSymbolType() == SymbolType::Value);
    REQUIRE(sym4->getSymbolType() == SymbolType::DataType);
    REQUIRE(sym5.getSymbolType() == SymbolType::Namespace);
    REQUIRE(sym6.getSymbolType() == SymbolType::Function);
    REQUIRE(sym7.getSymbolType() == SymbolType::Value);

    // they should all be different despite having the same name
    REQUIRE(sym0->getSymbolId() != sym1.getSymbolId());
    REQUIRE(sym0->getSymbolId() != sym2.getSymbolId());
    REQUIRE(sym0->getSymbolId() != sym3.getSymbolId());
    REQUIRE(sym0->getSymbolId() != sym4->getSymbolId());
    REQUIRE(sym0->getSymbolId() != sym5.getSymbolId());
    REQUIRE(sym0->getSymbolId() != sym6.getSymbolId());
    REQUIRE(sym0->getSymbolId() != sym7.getSymbolId());
    REQUIRE(sym1.getSymbolId() != sym2.getSymbolId());
    REQUIRE(sym1.getSymbolId() != sym3.getSymbolId());
    REQUIRE(sym1.getSymbolId() != sym4->getSymbolId());
    REQUIRE(sym1.getSymbolId() != sym5.getSymbolId());
    REQUIRE(sym1.getSymbolId() != sym6.getSymbolId());
    REQUIRE(sym1.getSymbolId() != sym7.getSymbolId());
    REQUIRE(sym2.getSymbolId() != sym3.getSymbolId());
    REQUIRE(sym2.getSymbolId() != sym4->getSymbolId());
    REQUIRE(sym2.getSymbolId() != sym5.getSymbolId());
    REQUIRE(sym2.getSymbolId() != sym6.getSymbolId());
    REQUIRE(sym2.getSymbolId() != sym7.getSymbolId());
    REQUIRE(sym3.getSymbolId() != sym4->getSymbolId());
    REQUIRE(sym3.getSymbolId() != sym5.getSymbolId());
    REQUIRE(sym3.getSymbolId() != sym6.getSymbolId());
    REQUIRE(sym3.getSymbolId() != sym7.getSymbolId());
    REQUIRE(sym4->getSymbolId() != sym5.getSymbolId());
    REQUIRE(sym4->getSymbolId() != sym6.getSymbolId());
    REQUIRE(sym4->getSymbolId() != sym7.getSymbolId());
    REQUIRE(sym5.getSymbolId() != sym6.getSymbolId());
    REQUIRE(sym5.getSymbolId() != sym7.getSymbolId());
    REQUIRE(sym6.getSymbolId() != sym7.getSymbolId());
    
    REQUIRE(sym0->getSymbolName() != sym1.getSymbolName());
    REQUIRE(sym0->getSymbolName() != sym2.getSymbolName());
    REQUIRE(sym0->getSymbolName() != sym3.getSymbolName());
    REQUIRE(sym0->getSymbolName() != sym4->getSymbolName());
    REQUIRE(sym0->getSymbolName() != sym5.getSymbolName());
    REQUIRE(sym0->getSymbolName() != sym6.getSymbolName());
    REQUIRE(sym0->getSymbolName() != sym7.getSymbolName());
    REQUIRE(sym1.getSymbolName() != sym2.getSymbolName());
    REQUIRE(sym1.getSymbolName() != sym3.getSymbolName());
    REQUIRE(sym1.getSymbolName() != sym4->getSymbolName());
    REQUIRE(sym1.getSymbolName() != sym5.getSymbolName());
    REQUIRE(sym1.getSymbolName() != sym6.getSymbolName());
    REQUIRE(sym1.getSymbolName() != sym7.getSymbolName());
    REQUIRE(sym2.getSymbolName() != sym3.getSymbolName());
    REQUIRE(sym2.getSymbolName() != sym4->getSymbolName());
    REQUIRE(sym2.getSymbolName() != sym5.getSymbolName());
    REQUIRE(sym2.getSymbolName() != sym6.getSymbolName());
    REQUIRE(sym2.getSymbolName() != sym7.getSymbolName());
    REQUIRE(sym3.getSymbolName() != sym4->getSymbolName());
    REQUIRE(sym3.getSymbolName() != sym5.getSymbolName());
    REQUIRE(sym3.getSymbolName() != sym6.getSymbolName());
    REQUIRE(sym3.getSymbolName() != sym7.getSymbolName());
    REQUIRE(sym4->getSymbolName() != sym5.getSymbolName());
    REQUIRE(sym4->getSymbolName() != sym6.getSymbolName());
    REQUIRE(sym4->getSymbolName() != sym7.getSymbolName());
    REQUIRE(sym5.getSymbolName() != sym6.getSymbolName());
    REQUIRE(sym5.getSymbolName() != sym7.getSymbolName());
    REQUIRE(sym6.getSymbolName() != sym7.getSymbolName());
}