#include "editor.h"
#include "roundedwidgets.h"

Editor::Editor(QWidget* parent)
    : QWidget(parent)
{
    editor = new ZTextEdit(this);
    editor->setBottomMargin(80);
    editor->setObjectName("editor");
    //    connect(editor, &ZTextEdit::textChanged, [this]() { emit contentChanged(QTextDocumentFragment(editor->document())); });

    auto toolBar = initToolBar();

    auto* layout = new QStackedLayout(this);
    layout->setStackingMode(QStackedLayout::StackAll);
    layout->addWidget(toolBar);
    layout->setAlignment(toolBar, Qt::AlignBottom);
    layout->addWidget(editor);
    editor->setFocus();
    setLayout(layout);
    connect(editor, &ZTextEdit::indexChanged, [this](const InnerIndex& ori, const InnerIndex& idx) { emit indexUpdated(idx); });
    setEnabled(false);
}
QWidget* Editor::initToolBar()
{
    auto* layer = new TransparentWidget(this);
    auto toolBox = initToolButtons(layer);
    layer->setObjectName("toolBarLayer");
    QVBoxLayout* layout = new QVBoxLayout(layer);
    layer->setLayout(layout);
    layout->addStretch();

    QHBoxLayout* bar = new QHBoxLayout(layer);
    layout->addLayout(bar);

    bar->addStretch();
    bar->addWidget(toolBox);
    bar->addStretch();

    return layer;
}
QWidget* Editor::initToolButtons(QWidget* parent)
{
    auto* box = new RoundedWidget(parent);
    box->setObjectName("toolBarBox");
    //    box->setAutoFillBackground(true);
    box->setRadius(15);
    box->setShadow(10);
    //    box->setMargin(10);
    QHBoxLayout* layout = new QHBoxLayout(box);
    box->setLayout(layout);

    //    layout->setMargin(15);

    auto bold = new QPushButton(QIcon(":/images/font-bold"), "", box),
         italic = new QPushButton(QIcon(":/images/font-italic"), "", box),
         underline = new QPushButton(QIcon(":/images/font-underline"), "", box),
         linethrough = new QPushButton(QIcon(":/images/font-linethrough"), "", box),
         splitline = new QPushButton(QIcon(":/images/splitline"), "", box),
         image = new QPushButton(QIcon(":/images/insimage"), "", box), url = new QPushButton(QIcon(":images/insurl"), "", box),
         list = new QPushButton(QIcon(":images/inslist"), "", box),
         olist = new QPushButton(QIcon(":images/insorderedlist"), "", box), code = new QPushButton(QIcon(":images/inscode"), "", box);
    for (auto i : { bold, italic, underline, linethrough, url })
        i->setCheckable(true);
    connect(bold, &QPushButton::toggled, editor, &ZTextEdit::fBold);
    connect(editor, &ZTextEdit::fBoldState, bold, &QPushButton::setChecked);
    connect(italic, &QPushButton::toggled, editor, &ZTextEdit::fItalic);
    connect(editor, &ZTextEdit::fItalicState, italic, &QPushButton::setChecked);
    connect(underline, &QPushButton::toggled, editor, &ZTextEdit::fUnderline);
    connect(editor, &ZTextEdit::fUnderlineState, underline, &QPushButton::setChecked);
    connect(linethrough, &QPushButton::toggled, editor, &ZTextEdit::fLinethrough);
    connect(editor, &ZTextEdit::fLinethroughState, linethrough, &QPushButton::setChecked);
    connect(image, &QPushButton::clicked, editor, &ZTextEdit::pInsImage);
    connect(url, &QPushButton::clicked, editor, &ZTextEdit::pInsUrl);
    connect(url, &QPushButton::toggled, editor, &ZTextEdit::fUrl);
    connect(editor, &ZTextEdit::pIsUrl, url, &QPushButton::setChecked);
    connect(list, &QPushButton::clicked, editor, &ZTextEdit::pInsList);
    connect(olist, &QPushButton::clicked, editor, &ZTextEdit::pInsOrderedList);
    connect(code, &QPushButton::clicked, editor, &ZTextEdit::pInsCode);
    connect(splitline, &QPushButton::clicked, editor, &ZTextEdit::pSplitline);

    auto newAction = [this](QKeySequence shortcut, bool b = true) {
        auto rt = new QAction;
        rt->setCheckable(b);
        rt->setShortcut(shortcut);
        this->addAction(rt);
        return rt;
    };

    connect(newAction(QKeySequence::Bold), &QAction::toggled, editor, &ZTextEdit::fBold);
    connect(newAction(QKeySequence::Underline), &QAction::toggled, editor, &ZTextEdit::fUnderline);
    connect(newAction(QKeySequence::Italic), &QAction::toggled, editor, &ZTextEdit::fItalic);

    layout->addWidget(bold), layout->addWidget(italic), layout->addWidget(underline), layout->addWidget(linethrough),
        layout->addWidget(splitline), layout->addWidget(image), layout->addWidget(url), layout->addWidget(list),
        layout->addWidget(olist), layout->addWidget(code);
    return box;
}
void Editor::reset()
{
    //    qDebug()<<"call Editor::reset()"<<editor;
    editor->blockSignals(true);
    editor->clear();
    this->setEnabled(false);
    editor->blockSignals(false);
    //    qDebug()<<"Editor::reset() return";
}
