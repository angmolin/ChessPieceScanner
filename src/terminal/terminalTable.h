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
     * Constructor por defecto
     */
    TerminalTableModel();

    /**
     * Constructor con un parametro de entrada
     */
    TerminalTableModel(const tableModel &model);

    /**
     * Verifica index de la tabla
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
     * Constructor con un parametro de entrada
     */
    TerminalTable(const table &termTable);

    /**
     * Añade una columna
     */
    void addColumn(tableRowColumn column);

    /**
     * Añade una fila
     */
    void addRow(tableRowColumn row);

    /**
     * Obtiene columnas
     */
    unsigned int getColumns() const;

    /**
     * Obtiene filas
     */
    unsigned int getRows() const;

    /**
     * Funcion con un argumento row
     */
    const tableRowColumn &getRow(unsigned int row) const;

    /**
     * Funcion con dos argumentos row y col
     */
    const std::wstring &getValue(unsigned int column, unsigned int row) const;

    /**
     * Funcion que retorna termTableCellMargin
     */
    unsigned int getCellMargin() const;

    /**
     * funcion que retorna termTableLeftPadding
     */
    unsigned int getLeftPadding() const;

    /**
     * Funcion que obtiene el TerminalTable
     */
    const TerminalTableModel &getModel() const;

    /**
     * Funcion que cambia el valor termTableCellMargin
     */
    void setCellMargin(const unsigned int cellMargin);

    /**
     * Funcion que cambia el valor termTableLeftPadding
     */
    void setLeftPadding(const unsigned int leftPadding);

    /**
     *  Funcion que cambia el valor TerminalTable
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