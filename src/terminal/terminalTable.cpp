#include "terminalTable.h"

TerminalTableModel::TerminalTableModel()
{
    wcscpy(this->model[0], L"\u250C");  // TOP_LEFT
    wcscpy(this->model[1], L"\u252C");  // TOP_MIDDLE
    wcscpy(this->model[2], L"\u2510");  // TOP_RIGHT
    wcscpy(this->model[3], L"\u251C");  // LEFT
    wcscpy(this->model[4], L"\u253C");  // MIDDLE
    wcscpy(this->model[5], L"\u2524");  // RIGHT
    wcscpy(this->model[6], L"\u2514");  // BOTTOM_LEFT
    wcscpy(this->model[7], L"\u2534");  // BOTTOM_MIDDLE
    wcscpy(this->model[8], L"\u2518");  // BOTTOM_RIGHT
    wcscpy(this->model[9], L"\u2500");  // HORIZONTAL
    wcscpy(this->model[10], L"\u2502"); // VERTICAL
}

TerminalTableModel::TerminalTableModel(const tableModel &model)
{
    int i, j;

    for (i = 0; i < TERMINAL_TABLE_MODEL_SIZE; i++)
        for (j = 0; j < TERMINAL_TABLE_MODEL_CHAR_SIZE; j++)
            this->model[i][j] = model[i][j];
}

const TerminalTableModel::specialChar_t &TerminalTableModel::operator[](std::size_t index) const
{
    if (index > 10)
        throw std::runtime_error("¡El indice solicitado está fuera de los limites del modelo de tabla!");

    return this->model[index];
}

TerminalTable::TerminalTable(const table &termTable) : termTable(termTable), termTableCellMargin(1), termTableLeftPadding(0) {}

void TerminalTable::addColumn(tableRowColumn column)
{
    unsigned int i = 0;

    while (i < this->termTable.size() && i < column.size())
    {
        this->termTable[i].push_back(column[i]);
        i++;
    }
}

void TerminalTable::addRow(tableRowColumn row)
{
    this->termTable.push_back(row);
}

unsigned int TerminalTable::getColumns() const
{
    return this->termTable[0].size();
}

unsigned int TerminalTable::getRows() const
{
    return this->termTable.size();
}

const TerminalTable::tableRowColumn &TerminalTable::getRow(unsigned int row) const
{
    return this->termTable[row];
}

const std::wstring &TerminalTable::getValue(unsigned int column, unsigned int row) const
{
    return this->termTable[row][column];
}

unsigned int TerminalTable::getCellMargin() const
{
    return this->termTableCellMargin;
}

unsigned int TerminalTable::getLeftPadding() const
{
    return this->termTableLeftPadding;
}

const TerminalTableModel &TerminalTable::getModel() const
{
    return this->termTableModel;
}

void TerminalTable::setCellMargin(const unsigned int cellMargin)
{
    this->termTableCellMargin = cellMargin;
}

void TerminalTable::setLeftPadding(const unsigned int leftPadding)
{
    this->termTableLeftPadding = leftPadding;
}

void TerminalTable::setModel(const TerminalTableModel &tableModel)
{
    this->termTableModel = tableModel;
}

void drawTableBar(std::wostream &os, const unsigned int columns, const unsigned int *sizes, const unsigned int cellMargin, const TerminalTableModel::specialChar_t firstChar, const TerminalTableModel::specialChar_t fillChar, const TerminalTableModel::specialChar_t middleChar, const TerminalTableModel::specialChar_t lastChar)
{
    unsigned int i, j;
    unsigned int size = 0;

    os << firstChar;
    size++;

    for (i = 0; i < columns; i++)
    {
        for (j = 0; j < sizes[i] + cellMargin * 2; j++) {
            os << fillChar;
            size++;
        }

        if (i < columns - 1) {
            os << middleChar;
            size++;
        }
    }

    os << lastChar;
    size++;

    for (i = 0; i < size; i++)
        os << "\033[D";

    os << "\033[B";
}

void drawTableContent(std::wostream &os, const unsigned int *sizes, const unsigned int cellMargin, const std::vector<std::wstring> row, const TerminalTableModel::specialChar_t firstChar, const TerminalTableModel::specialChar_t fillChar, const TerminalTableModel::specialChar_t middleChar, const TerminalTableModel::specialChar_t lastChar)
{
    unsigned int i, j;
    unsigned int size = 0;

    os << firstChar;
    size++;

    for (i = 0; i < row.size(); i++)
    {
        for (j = 0; j < cellMargin; j++) {
            os << fillChar;
            size++;
        }

        os << row[i];
        size += row[i].length();

        for (j = 0; j < cellMargin; j++) {
            os << fillChar;
            size++;
        }

        for (j = 0; j < sizes[i] - row[i].length(); j++) {
            os << fillChar;
            size++;
        }

        if (i < row.size() - 1) {
            os << middleChar;
            size++;
        }
    }

    os << lastChar;
    size++;

    for (i = 0; i < size; i++)
        os << "\033[D";

    os << "\033[B";
}

inline void drawTableTopbar(std::wostream &os, const unsigned int columns, const unsigned int *sizes, const unsigned int cellMargin, const TerminalTableModel &model)
{
    drawTableBar(os, columns, sizes, cellMargin, model[0], model[9], model[1], model[2]);
}

inline void drawTableMiddlebar(std::wostream &os, const unsigned int columns, const unsigned int *sizes, const unsigned int cellMargin, const TerminalTableModel &model)
{
    drawTableBar(os, columns, sizes, cellMargin, model[3], model[9], model[4], model[5]);
}

inline void drawTableBottombar(std::wostream &os, const unsigned int columns, const unsigned int *sizes, const unsigned int cellMargin, const TerminalTableModel &model)
{
    drawTableBar(os, columns, sizes, cellMargin, model[6], model[9], model[7], model[8]);
}

std::wostream &operator<<(std::wostream &os, const TerminalTable &termTable)
{
    unsigned int i, j;

    unsigned int columns = termTable.getColumns();
    unsigned int rows = termTable.getRows();

    unsigned int *columnSize = (unsigned int *)malloc(sizeof(unsigned int) * columns);

    for (i = 0; i < columns; i++)
    {
        columnSize[i] = 0;

        for (j = 0; j < rows; j++)
        {
            unsigned int columnISize = termTable.getValue(i, j).length();

            if (columnISize > columnSize[i])
                columnSize[i] = columnISize;
        }
    }

    for (i = 0; i < termTable.getLeftPadding(); i++)
        os << "\033[C";

    drawTableTopbar(os, columns, columnSize, termTable.getCellMargin(), termTable.getModel());

    for (i = 0; i < rows; i++)
    {
        drawTableContent(os, columnSize, termTable.getCellMargin(), termTable.getRow(i), termTable.getModel()[10], L" ", termTable.getModel()[10], termTable.getModel()[10]);

        if (i < rows - 1)
            drawTableMiddlebar(os, columns, columnSize, termTable.getCellMargin(), termTable.getModel());
    }

    drawTableBottombar(os, columns, columnSize, termTable.getCellMargin(), termTable.getModel());

    free(columnSize);

    return os;
}