# CBase

A lightweight relational database engine built from scratch in C.

CBase provides a simple SQL-like command-line interface for creating tables, storing records, querying data, updating records, deleting records, and persisting the database to disk.

## Features

- Interactive command-line interface
- Multiple tables
- Dynamic table schemas
- Supported data types:
  - `INT`
  - `FLOAT`
  - `TEXT`
- `CREATE TABLE`
- `INSERT`
- `SELECT`
- `WHERE` conditions
- `UPDATE`
- `DELETE`
- Persistent storage
- Serialization and deserialization
- Simple page abstraction
- Basic error handling

## Architecture

```text
                    +----------------+
                    |   CBase CLI    |
                    +-------+--------+
                            |
                            v
                    +----------------+
                    |     Parser     |
                    +-------+--------+
                            |
                            v
                    +----------------+
                    |    Executor    |
                    +---+----+----+--+
                        |    |    |
                        v    v    v
                    +----+ +---+ +------+
                    |Table| |Row| |Value |
                    +----+ +---+ +------+
                        |
                        v
                  +-------------+
                  |   Storage   |
                  +------+------+
                         |
                         v
                    cbase.db
```

### Components

#### CLI

Handles user interaction through the CBase REPL.

Responsibilities:

- Read commands
- Recognize meta commands
- Route SQL commands to the parser
- Display results and errors

#### Parser

Converts SQL-like commands into structured query objects.

For example:

```sql
SELECT * FROM students WHERE age > 20
```

is converted into a structured `SelectQuery`.

#### Executor

Performs operations on the database structures.

It handles:

- SELECT
- UPDATE
- DELETE
- WHERE condition evaluation

#### Table

Represents a database table.

A table contains:

- Table name
- Column definitions
- Rows
- Row count
- Row capacity

#### Row

Represents one record in a table.

Each row contains an array of `Value` objects.

#### Value

Represents a typed value.

CBase supports:

```text
INT
FLOAT
TEXT
```

#### Storage

Responsible for persistence.

The database is serialized into:

```text
data/cbase.db
```

When CBase starts, the file is loaded back into memory.

When CBase exits, the current database is written back to disk.

## Project Structure

```text
CBase/
├── README.md
├── Makefile
├── .gitignore
│
├── include/
│   ├── common.h
│   ├── cli.h
│   ├── database.h
│   ├── table.h
│   ├── row.h
│   ├── value.h
│   ├── parser.h
│   ├── executor.h
│   ├── pager.h
│   └── storage.h
│
├── src/
│   ├── main.c
│   ├── cli.c
│   ├── database.c
│   ├── table.c
│   ├── row.c
│   ├── value.c
│   ├── parser.c
│   ├── executor.c
│   ├── pager.c
│   └── storage.c
│
├── tests/
│   ├── test_parser.c
│   ├── test_storage.c
│   ├── test_table.c
│   └── test_queries.c
│
├── docs/
│   ├── architecture.md
│   ├── storage-format.md
│   └── query-processing.md
│
└── data/
    └── cbase.db
```

## Building

CBase requires a C compiler supporting C11.

Build the project with:

```bash
make
```

This produces:

```text
cbase
```

Run it using:

```bash
./cbase
```

## Example

Create a table:

```sql
CREATE TABLE students (id INT, name TEXT, age INT, cgpa FLOAT)
```

Insert records:

```sql
INSERT INTO students VALUES (1, 'Goyam', 20, 9.47)
INSERT INTO students VALUES (2, 'Rahul', 21, 8.90)
```

Query records:

```sql
SELECT * FROM students
```

Use a condition:

```sql
SELECT * FROM students WHERE age > 20
```

Update records:

```sql
UPDATE students SET age = 22 WHERE id = 2
```

Delete records:

```sql
DELETE FROM students WHERE id = 2
```

List tables:

```text
.tables
```

View a table schema:

```text
.schema students
```

Exit:

```text
.exit
```

## Persistence

CBase stores its database on disk in:

```text
data/cbase.db
```

The database is loaded when CBase starts and saved when CBase exits.

The storage layer serializes the database logically instead of writing pointer-containing C structures directly to disk.

The stored information includes:

```text
Database
 ├── Tables
 │    ├── Table name
 │    ├── Columns
 │    │    ├── Column name
 │    │    └── Data type
 │    └── Rows
 │         └── Values
```

## Design Decisions

CBase intentionally focuses on a small and understandable database architecture.

The project does not implement advanced database features such as:

- B-tree indexes
- Query optimization
- Transactions
- Write-ahead logging
- Concurrent transactions
- Joins
- A full SQL grammar

These features are outside the scope of the project.

The goal is to understand the fundamental components involved in building a database engine:

```text
Parsing
   ↓
Query execution
   ↓
Tables and rows
   ↓
Typed values
   ↓
Serialization
   ↓
Persistent storage
```

## Limitations

Current limitations include:

- Maximum number of tables is limited
- Maximum number of columns per table is limited
- Maximum text length is limited
- Only a subset of SQL is supported
- Queries operate primarily through in-memory table structures
- No indexes are implemented
- No transaction system is implemented
- No concurrent access support

## Future Improvements

Possible future extensions include:

- B-tree indexes
- More SQL operations
- `ORDER BY`
- `LIMIT`
- Multiple conditions using `AND` / `OR`
- Aggregate functions
- JOIN operations
- Transactions
- Better page management
- Query optimization
- Concurrency support

## Learning Objectives

CBase demonstrates practical concepts in:

- C programming
- Structures and pointers
- Dynamic memory allocation
- File I/O
- Serialization
- Parsing
- Data structures
- Query processing
- Memory management
- Software architecture

## License
This project is licensed under the MIT License — see the [LICENSE](LICENSE) file for details.