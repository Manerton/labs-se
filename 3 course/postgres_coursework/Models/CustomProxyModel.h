#ifndef CUSTOMPROXYMODEL_H
#define CUSTOMPROXYMODEL_H
#include <QObject>
#include <QSortFilterProxyModel>
#include <QColor>

class CustomProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

    std::list<int> cart; // корзина
public:
    explicit CustomProxyModel(QObject *parent = nullptr)
        : QSortFilterProxyModel(parent)
    {}

    // добавляем подсветку строки в таблице, если товар в корзине
    QVariant data(const QModelIndex &index, int role) const override
    {
        if (role == Qt::BackgroundRole)
        {
            int row_id = QSortFilterProxyModel::data(this->index(index.row(), 0)).toInt();
            if (std::find(cart.begin(), cart.end(), row_id) != cart.end())
            {
                // если товар в корзине, то строку подсветить
                return QColor(255,174,66);
            }
        }
        else if (role == Qt::DisplayRole)
        {
            return QSortFilterProxyModel::data(index);
        }
        return QVariant();
    }
    // добавление в корзину
    void insertInCart(int id_product)
    {
        if (id_product)
        {
            if (std::find(cart.begin(), cart.end(), id_product) == cart.end())
            {
                cart.push_back(id_product);
            }
        }
    }
    // удаление из корзины
    void removeFromCart(int id_product)
    {
        if (id_product)
        {
            cart.remove(id_product);
        }
    }

    void clearCart()
    {
        cart.clear();
    }

    const std::list<int> &getCart() const;
};

inline const std::list<int>& CustomProxyModel::getCart() const
{
    return cart;
}

#endif // CUSTOMPROXYMODEL_H
