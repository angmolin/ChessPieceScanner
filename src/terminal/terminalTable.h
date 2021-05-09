#ifndef TERMINAL_TABLE_H
#define TERMINAL_TABLE_H

#include <ostream>
#include <stdexcept>
#include <vector>

#include <string.h>

class TerminalTableModel
{
#define TERMINAL_TABLE_MODEL_CHAR_SIZE 2
#define TERMINAL_TABLE_MODEL_SIZE 11
public:
    typedef wchar_t specialChar_t[TERMINAL_TABLE_MODEL_CHAR_SIZE];
    typedef specialChar_t tableModel[TERMINAL_TABLE_MODEL_SIZE];

    /**
     * Crea un nuevo objeto WebcamV4L2
     */
    TerminalTableModel();

    /**
     * Crea un nuevo objeto WebcamV4L2
     */
    TerminalTableModel(const tableModel &model);

    /**
     * Crea un nuevo objeto WebcamV4L2
     */
    const specialChar_t &operator[](std::size_t index) const;

private:
    tableModel model;
};

class TerminalTable
{
public:
    typedef std::vector<std::wstring> tableRowColumn;
    typedef std::vector<tableRowColumn> table;

    /**
     * Crea un nuevo objeto WebcamV4L2
     */
    TerminalTable(const table &termTable);

    /**
     * Crea un nuevo objeto WebcamV4L2
     */
    void addColumn(tableRowColumn column);

    /**
     * Crea un nuevo objeto WebcamV4L2
     */
    void addRow(tableRowColumn row);

    /**
     * Crea un nuevo objeto WebcamV4L2
     */
    unsigned int getColumns() const;

    /**
     * Crea un nuevo objeto WebcamV4L2
     */
    unsigned int getRows() const;

    /**
     * Crea un nuevo objeto WebcamV4L2
     */
    const tableRowColumn &getRow(unsigned int row) const;

    /**
     * Crea un nuevo objeto WebcamV4L2
     */
    const std::wstring &getValue(unsigned int column, unsigned int row) const;

    /**
     * Crea un nuevo objeto WebcamV4L2
     */
    unsigned int getCellMargin() const;

    /**
     * Crea un nuevo objeto WebcamV4L2
     */
    unsigned int getLeftPadding() const;

    /**
     * Crea un nuevo objeto WebcamV4L2
     */
    const TerminalTableModel &getModel() const;

    /**
     * Crea un nuevo objeto WebcamV4L2
     */
    void setCellMargin(const unsigned int cellMargin);

    /**
     * Crea un nuevo objeto WebcamV4L2
     */
    void setLeftPadding(const unsigned int leftPadding);

    /**
     * Crea un nuevo objeto WebcamV4L2
     */
    void setModel(const TerminalTableModel &tableModel);

private:
    TerminalTableModel termTableModel;
    table termTable;
    unsigned int termTableCellMargin;
    unsigned int termTableLeftPadding;
};

std::wostream &operator<<(std::wostream &os, const TerminalTable &obj);

#endif