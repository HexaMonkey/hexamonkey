#include "treewidget.h"

TreeWidget::TreeWidget(const InterpreterConstructor& interpreterConstructor, QWidget *parent) :
    QWidget(parent), interpreterConstructor(interpreterConstructor)
{
    setAcceptDrops(true);

    view =  new TreeView(this);
    model = new TreeModel("Tree", interpreterConstructor, this);

    view->setModel(model);

    delegate = new HTMLDelegate(this);

    view->setModel(model);
    view->setItemDelegate(delegate);
    setColumnsWidths();

    filterWidget = new FilterWidget(this);

    //Layout
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(view, 0, 0, 1, 1);
    layout->addWidget(filterWidget, 1, 0, 1, 1);
    layout->setContentsMargins(0,0,0,0);
    setLayout(layout);

    connect(view, SIGNAL(selected(QModelIndex)), model, SLOT(updateCurrent(QModelIndex)));
    connect(view, SIGNAL(expanded(QModelIndex)), model, SLOT(requestExpansion(QModelIndex)));

    connect(view, SIGNAL(selected(QModelIndex)), this, SLOT(updatePath(QModelIndex)));
    connect(view, SIGNAL(selected(QModelIndex)), this, SLOT(updatePosition(QModelIndex)));

    connect(model, SIGNAL(filterChanged(QString)), filterWidget, SLOT(changeText(QString)));
    connect(filterWidget, SIGNAL(textChanged(QString)), model, SLOT(updateFilter(QString)));
    connect(model, SIGNAL(invalidFilter()), filterWidget, SLOT(invalidateText()));
}

void TreeWidget::dragEnterEvent(QDragEnterEvent* event)
{
    // if some actions should not be usable, like move, this code must be adopted
    event->acceptProposedAction();
}

void TreeWidget::dragMoveEvent(QDragMoveEvent* event)
{
    // if some actions should not be usable, like move, this code must be adopted
    event->acceptProposedAction();
}


void TreeWidget::dragLeaveEvent(QDragLeaveEvent* event)
{
    event->accept();
}

void TreeWidget::dropEvent(QDropEvent *event)
{
    QWidget::dropEvent(event);
    std::cerr<<"eventDrop"<<std::endl;
    emit eventDropped(event);
}

QSize TreeWidget::sizeHint() const
{
    return QSize(800,800);
}

void TreeWidget::setColumnsWidths()
{
    int w = sizeHint().width();
    view->setColumnWidth(0, w - 250);
    view->setColumnWidth(1, 100);
    view->setColumnWidth(2, 120);
}

QModelIndex TreeWidget::addObject(Object& object)
{
    model->addResource(object);
    return model->addObject(object);
}

void TreeWidget::updatePath(QModelIndex currentIndex)
{
    QString newPath = model->path(currentIndex);
    if(newPath != path)
    {
        emit pathChanged(newPath);
        path = newPath;
    }
}

void TreeWidget::updatePosition(QModelIndex currentIndex)
{
    quint64 newPosition = model->position(currentIndex);
    quint64 newSize     = model->size(currentIndex);


    if(newPosition != position || newSize != size)
    {
        emit positionChanged(newPosition, newSize);
        position = newPosition;
        size     = newSize;
    }
}

void TreeWidget::setCurrentIndex(QModelIndex index)
{
    return view->setCurrentIndex(index);
}
