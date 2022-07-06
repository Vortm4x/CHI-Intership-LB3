#ifndef SALE_H
#define SALE_H

#include "Product.h"
#include <list>
#include <memory>
#include <typeinfo>

namespace sict
{
    class Sale
    {
    using TaxableProductShared = std::shared_ptr<TaxableProduct*>;

    public:
        Sale();
        Sale(const char* path);
        ~Sale();

    void display(std::ostream & os) const;

    private:
        std::list<iProduct*> records;
    };

    Sale::Sale()
    {}

    Sale::Sale(const char* path)
    {
        std::ifstream file(path);

        if(file.is_open() )
        {
            TaxableProduct input;
            iProduct* current;
            while(true)
            {
                current = input.readRecord(file);
                if(!file.good() ) break;

                records.push_back(current);
            }

            delete current;
        }
        else
        {

        }
    }

    void Sale::display(std::ostream & os) const
    {
        os << std::setw(iProduct::fieldWidth) << "Product No"
           << std::setw(iProduct::fieldWidth) << "Cost Taxable";
        os << '\n';

        double total = 0.0;

        for(const auto& rec : records)
        {
            total += rec->price();

            rec->display(os);
            os << '\n';
        }

        os << std::setw(iProduct::fieldWidth) << "Total"
           << std::setw(iProduct::fieldWidth) << std::setprecision(2) << std::fixed << total;
        os << '\n';
    }

    Sale::~Sale()
    {
        for(auto& rec : records)
        {
            delete rec;
        }
    }

}



#endif // SALE_H
