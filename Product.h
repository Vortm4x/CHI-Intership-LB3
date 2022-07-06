#ifndef PRODUCT_H
#define PRODUCT_H

#include <fstream>
#include <iomanip>
#include <algorithm>
#include <map>
#include <sstream>

namespace sict
{
    class iProduct
    {
    public:
        iProduct(){}
        virtual double price() const = 0;
        virtual void display(std::ostream& os) const = 0;
        virtual iProduct* readRecord(std::ifstream& file) = 0;
        virtual ~iProduct(){}

        friend std::ostream& operator<<(std::ostream& os, const iProduct& p);

        static int fieldWidth;
    };




    class Product : public iProduct
    {
    public:

        Product();
        Product(const uint& _productNo, const double& _productPrice);
        virtual ~Product();

        virtual double price() const override;
        virtual void display(std::ostream& os) const override;
        virtual iProduct* readRecord(std::ifstream& file) override;

        friend std::ostream& operator<<(std::ostream & os, const iProduct& p);

    protected:
        uint productNo = 0;
        double productPrice = 0;
    };

    Product::Product()
        : iProduct()
    {}

    Product::Product(const uint& _productNo, const double& _productPrice)
        : iProduct(), productNo(_productNo), productPrice(_productPrice)
    {}

    Product::~Product()
    {}

    double Product::price() const
    {
        return productPrice;
    }

    void Product::display(std::ostream& os) const
    {
        os << std::setw(fieldWidth) << productNo;
        os << std::setw(fieldWidth) << std::setprecision(2) << std::fixed << productPrice;
    }

    iProduct* Product::readRecord(std::ifstream& file)
    {
        Product* product = new Product();
        file >> product->productNo >> product->productPrice;

        return dynamic_cast<iProduct*>(product);
    }

    std::ostream& operator<<(std::ostream & os, const iProduct& p)
    {
        p.display(os);
        return os;
    }




    class TaxableProduct : public Product
    {
    public:
        using Tax = std::pair<char, double>;

        TaxableProduct();
        TaxableProduct(const uint& _productNo, const double& _productPrice, const char& _taxSign);
        virtual ~TaxableProduct();

        virtual double price() const override;
        virtual void display(std::ostream& os) const override;
        iProduct* readRecord(std::ifstream & file) override;

    protected:
        char taxSign = 0;
        static std::map<char, double> taxTypes;
    };


    std::map<char, double> TaxableProduct::taxTypes {
        { 'H', 0.13 },
        { 'P', 0.08 },
    };


    TaxableProduct::TaxableProduct()
        : Product()
    {}

    TaxableProduct::TaxableProduct(const uint& _productNo, const double& _productPrice, const char& _taxSign)
        : Product(_productNo, _productPrice), taxSign(_taxSign)
    {}

    TaxableProduct::~TaxableProduct()
    {}

    double TaxableProduct::price() const
    {
        auto it = taxTypes.find(taxSign);
        double tax = 0.0;
        if(it != taxTypes.cend()) tax = it->second;

        return Product::productPrice * (1.0 + tax);
    }

    void TaxableProduct::display(std::ostream& os) const
    {
        Product::display(os);
        os << std::setw(iProduct::fieldWidth) << taxSign << "ST";
    }

    iProduct* TaxableProduct::readRecord(std::ifstream & file)
    {
        std::stringstream sstream;
        std::string record;

        std::getline(file, record);
        sstream << record;

        uint productNo = 0;
        double productPrice = 0;
        char taxSign = 0;

        sstream >> productNo >> productPrice >> taxSign;

        if(taxSign != 0)
        {
            return dynamic_cast<iProduct*>(new TaxableProduct(productNo, productPrice, taxSign) );
        }
        else
        {
            return dynamic_cast<iProduct*>(new Product(productNo, productPrice) );
        }
    }

}


#endif // PRODUCT_H
