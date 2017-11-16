#include "memorycomponent.h"
#include <QtGlobal>
#include <QDialog>
#include <QDebug>
#include <QTextEdit>
#include <QFontDatabase>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include "byteinputterminal.h"
#include "bitinputterminal.h"
#include "byteoutputterminal.h"
#include <QComboBox>
#include "myresources.h"
#include "mainwindow.h"
#include <QStatusBar>
#include <QCheckBox>

MemoryComponent::MemoryComponent()
{
    combinational = false;

    // initialize data
    setData("");

    readWrite = addBitInputTerminal("read/write");
    index = addByteInputTerminal("index"); // the index is a short int that points to a byte in memory
    dataIn = addByteInputTerminal("data in");
    dataOut = addByteOutputTerminal("data out");
    // for example, an assembly instruction for adding two registers would be
    // (will it always be 3 (or less?) operands?
    // ADD 1 2 0
    // which will add the values in R2 and R0 and put the output in R1
    // and in machine code this will look like
    // 0000 0001 0000 0001 0000 0010 0000 0000
    // \ instr / \   R1  / \  R2   / \  R0   /
    // I was actually thinking I wouldn't need registers
    // but it would be kind of silly not to have them
    // but actually I can just use a memory component as a register array
    // the registers should be 16-bit, because that's what I made the ALU
    // actually this is wrong, a whole instruction would be 7 bytes

    // the only other thing you need to be able to do is write literals
    // so I'll have to have some convention

    // so yeah, I really don't think I'll imlement a real instruction set architecture
    // but I think I am gonna need to make my own assembler inside this component
    // So there will be a checkbox in the properties which says "assemble text"
    // then when you finish editing the text it will be assembled into assembledData
    // and then the QByteArray used will be assembledData instead of data
    // and maybe display mode will show the assembled data in hex or binary (or an error message)

    // hmm, this text isn't really fixed width (or height)
    myfont.setFamily("courier");
    myfont.setStyleHint(QFont::Monospace);

    image = &myresources->memory;
}


void MemoryComponent::createSettings()
{
    QComboBox * comboBox = CreateComboBoxSetting(this,SLOT(stateChanged(int)));
    comboBox->addItem("No Display");
    comboBox->addItem("Text Mode");
    comboBox->addItem("Hex Mode");
    comboBox->setCurrentIndex(state);

    CreateCheckBoxSetting("assemble",assemble,this,SLOT(assembleChanged(int)));
}

QString MemoryComponent::SerializeSettings()
{
    // by turning into hex I'm hoping to avoid problems with invalid latin1 characters (+ helps it fit on one line)
    QByteArray hex = data.toHex();
    QString serialized = QString::fromLatin1(hex);
    return QString::number(state) + "," +
           QString::number(assemble) + "," +
           QString::number(size) + "," +
           serialized;
}

void MemoryComponent::DeserializeSettings(QString string)
{
    QStringList split = string.split(',');
    stateChanged(split[0].toInt());
    assembleChanged(split[1].toInt());

    QByteArray hex = split[3].toLatin1();
    setData(QByteArray::fromHex(hex));
}

void MemoryComponent::ReevaluateLogic()
{
    unsigned char i = index->getValue();
    if (!assemble)
    {
        if (!readWrite->getValue())
        {
            // read at index
            dataOut->setValue(data[i]);
        }
        else
        {
            // write dataIn to data at index
            data[i] = dataIn->getValue();
            updateDisplayData();
            update();
        }
    }
    else
    {
        // index is now the line number; machine code instructions are 7 bytes long
        i *= 4;

        // it would be nice to be able to see the machine code...
        opcode->setValue(machineCode[i]);
        param1->setValue(machineCode[i+1]);
        param2->setValue(machineCode[i+2]);
        param3->setValue(machineCode[i+3]);
    }
}

void MemoryComponent::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    // open text editor thing
    QDialog dialog;
    dialog.setAccessibleName("Edit Data");
    QVBoxLayout vLayout;
    QTextEdit textEdit;
    vLayout.addWidget(&textEdit);
    dialog.setLayout(&vLayout);

    textEdit.setCurrentFont(myfont);
    textEdit.setPlainText(QString::fromLatin1(data));
    // textEdit.setWordWrapMode(QTextOption::WrapAnywhere);
    // maybe text wrap should be a checkbox. we might want it on

    // I was gonna make it fixed size, so it would be the same as display mode
    // but tbh that would just be confusing
    QHBoxLayout hLayout;
    vLayout.addLayout(&hLayout);
    QPushButton okButton;
    okButton.setText("&Ok");
    hLayout.addWidget(&okButton);
    QObject::connect(&okButton,SIGNAL(clicked()),&dialog,SLOT(accept()));
    QPushButton cancelButton;
    cancelButton.setText("&Cancel");
    hLayout.addWidget(&cancelButton);
    QObject::connect(&cancelButton,SIGNAL(clicked()),&dialog,SLOT(close()));

    int code = dialog.exec();

    if (code) // if code = 1
    {
        QByteArray text;
        text = textEdit.toPlainText().toLatin1();

        if (state == 2 && !assemble) // bit hacky
            text = QByteArray::fromHex(text);

        setData(text);
    }
}

void MemoryComponent::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    // overrides default paint algorithm because reasons
    if (option->state & QStyle::State_Selected)
        pen.setColor(Qt::yellow);
    else
        pen.setColor(Qt::black);

    painter->setPen(pen);
    painter->drawRect(rect);
    painter->fillRect(rect,QBrush(QColor(Qt::white)));

    switch (state)
    {
    case 0:
        painter->drawPixmap(-16,-16,32,32,*image);
        break;
    case 1:
    case 2:
        painter->setPen(QPen());
        painter->setFont(myfont);
        painter->drawText(-120,-120,240,240,Qt::AlignLeft | Qt::TextWrapAnywhere,QString::fromLatin1(displayData));
        break;
    }
}

void MemoryComponent::updateDisplayData()
{
    // update display data
    // remove any bad characters (this could get expensive...)
    // we should really stop it from doing this too many times per second

    if (state == 1)
    {
        // hmm, maybe don't bother with this
//        displayData.clear();
//        displayData.resize(size);
//        for (int i = 0; i < size; i++)
//            displayData[i] = validateChar(data[i]);

        displayData = data;
    }
    else if (state == 2)
    {
        if (assemble)
            displayData = machineCode.toHex();
        else
            displayData = data.toHex();
    }
    if (state != 0)
        displayData.truncate(540);
}

void MemoryComponent::setData(QByteArray _data)
{
    if (_data.size() > size)
        _data.truncate(size);
    else if (_data.size() < size)
    {
        if (state == 1) // text mode
        {
            // fill out with spaces
            int difference = size - _data.size();
            QByteArray spaces = QByteArray(difference,' ');
            _data.append(spaces);
        }
        else if (state == 2)
        {
            // fill out with zeros
            int difference = size - _data.size(); // size*2? this is rubbish
            QByteArray spaces = QByteArray(difference,0);
            _data.append(spaces);
        }
    }

    data = _data;

    if (assemble)
        assembleData();

    updateDisplayData();
}

char MemoryComponent::validateChar(char _char)
{
    if (mywindow->allowedChars.contains(_char))
        return _char;
    else
        return ' ';
}

void MemoryComponent::assembleData()
{
    // try to assemble data into assembledData
    // this is an assembler
    // there are four passes
    // it supports blank lines, comments and labels
    // doesn't really do any error checking (if you put in dummy labels or something)
    // haven't really tested it but it seems ok
    QList<QByteArray> lines = data.split('\n');

    // first pass removes comments and blank lines (and splits the lines)
    QMutableListIterator<QByteArray> iter(lines);
    QList<QList<QByteArray>> splitLines;
    while (iter.hasNext())
    {
        QList<QByteArray> splitLine = removeEmpty(iter.next().split(' '));
        if (splitLine.isEmpty())
        {
            // blank line
            iter.remove();
            continue;
        }

        if (splitLine[0].startsWith('#'))
        {
            // comment
            iter.remove();
            continue;
        }

        // otherwise, add split line to list
        splitLines << splitLine;
    }

    // second pass we add labels to token list
    QHash<QByteArray,int> tokens; // label and line number
    int lineNumber = 0;
    QMutableListIterator<QList<QByteArray>> splitIter(splitLines);
    while (splitIter.hasNext())
    {
        // remove spaces
        QByteArray firstElement = splitIter.next()[0];
        if (firstElement.endsWith(':'))
        {
            // this is a label, remove it
            splitIter.remove();
            // remove colon and add token to list
            firstElement.remove(firstElement.size()-1,1);
            tokens.insert(firstElement,lineNumber);
        }
        else
            // increment line number count
            lineNumber++;
    }

    // replace tokens with line numbers
    for (int i = 0; i < splitLines.size(); i++)
    {
        QList<QByteArray> splitLine = splitLines[i];

        // for each split
        for (int j = 0; j < splitLine.size(); j++)
        {
            // if split == a token
            if (tokens.contains(splitLine[j]))
                // replace token with line
                splitLine[j] = QString::number(tokens.value(splitLine[j])).toLatin1();
        }

        // assign line back to splitLines
        splitLines[i] = splitLine;
    }

    // final pass, make machine code
    // an instruciton should be in the form:
    // ADD 1 2 0
    // an instruction, followed by 3 16 bit numbers, all delimited by spaces
    for (int i = 0; i < splitLines.size(); i++)
    {
        QList<QByteArray> splitLine = splitLines[i];

        QByteArray instruction = splitLine[0];

        unsigned char param1 = 0;
        unsigned char param2 = 0;
        unsigned char param3 = 0;

        if (splitLine.size() > 1)
            // may be empty
            param1 = splitLine[1].toUShort();

        if (splitLine.size() > 2)
            param2 = splitLine[2].toUShort();

        if (splitLine.size() > 3)
            param3 = splitLine[3].toUShort();

        // encode
        int j = i*4; // an instruction is 4 bytes long
        int opcode = mywindow->instructions.indexOf(instruction);
        if (opcode < 0)
        {
            // assembly failed
            mywindow->statusBar()->showMessage("Assembly failed. Unrecognied instruction: " + instruction);
            return;
        }
        else
        {
            machineCode[j] = opcode; // implicitly truncated
        }

        machineCode[j+1] = param1;
        machineCode[j+2] = param2;
        machineCode[j+3] = param3;
    }

    mywindow->statusBar()->showMessage("Assembly successful");
}

QList<QByteArray> MemoryComponent::removeEmpty(QList<QByteArray> _list)
{
    QMutableListIterator<QByteArray> iter(_list);
    while (iter.hasNext())
        if (iter.next().trimmed().isEmpty())
            iter.remove();
    return _list;
}

void MemoryComponent::stateChanged(int _state)
{
    if (state == _state) return;

    state = _state;

    switch (state)
    {
    case 0:
        fixedHeight = false;
        width = 40;
        break;
    case 1:
    case 2:
        fixedHeight = true;
        width = 260;
        height = 260;
        setData(data); // causes display data to be updated
        break;
    }

    resize();
}

void MemoryComponent::assembleChanged(int _assemble)
{
    if (assemble == _assemble) return;
    assemble = _assemble;

    if (assemble)
    {
        // remove dataOut
        removeByteOutputTerminal();

        // add instruction outputs
        opcode = addByteOutputTerminal("opcode");
        param1 = addByteOutputTerminal("first parameter");
        param2 = addByteOutputTerminal("second parameter");
        param3 = addByteOutputTerminal("third parameter");
    }
    else
    {
        // the opposite
        for (int i = 0; i < 4; i++)
            removeByteOutputTerminal();

        dataOut = addByteOutputTerminal("data out");
    }
    assembleData();
}
