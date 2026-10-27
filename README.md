# 🎨 Source to HTML Converter in C

A modular **C-based Source-to-HTML Converter** that transforms C source code into a browser-viewable HTML page with CSS-based syntax highlighting.

The program scans the input source, identifies common C language elements, and writes them into an HTML document. The generated page uses a stylesheet to make code easier to read.

## ✨ Features

- Converts a C source file into an HTML document.
- Highlights C keywords, identifiers, numbers, strings, and character literals.
- Styles comments, preprocessor lines, operators, and punctuation.
- Preserves source whitespace and line breaks.
- Escapes HTML-sensitive characters such as `<`, `>`, `&`, and `"`.
- Accepts an input file and an optional output filename.
- Generates a browser-ready HTML page linked to `styles.css`.
- Uses a modular implementation with separate conversion and scanning components.

## 🧰 Built With

- **Language:** C
- **Markup:** HTML
- **Styling:** CSS
- **Compiler:** GCC / MinGW
- **Build tool:** Make (optional)

## 📁 Project Structure

```text
source_to_html/
├── s2html_main.c    # Command-line entry point
├── s2html_conv.c    # File conversion and HTML document generation
├── s2html_conv.h    # Conversion function declaration
├── s2html_event.c   # Source scanning and HTML-safe output
├── s2html_event.h   # Event/token definitions
├── styles.css       # Syntax highlighting and page styling
├── sample.c        # Sample C source file
├── Makefile        # Build and clean commands
└── README.md
```

## ⚙️ Build and Run

### Windows (GCC / MinGW)

Open a terminal in the project directory and compile:

```bash
gcc -std=c11 -Wall -Wextra -Wpedantic -O2 s2html_main.c s2html_event.c s2html_conv.c -o s2html.exe
```

Convert the included sample file:

```powershell
.\s2html.exe sample.c
```

This generates:

```text
sample.c.html
```

### Linux / macOS

Build using the included Makefile:

```bash
make
```

Run:

```bash
./s2html sample.c
```

To remove generated files:

```bash
make clean
```

## ▶️ Convert Your Own Source File

Provide the source file path:

```bash
./s2html input.c
```

By default, the generated output will be named:

```text
input.c.html
```

You can also specify an output filename:

```bash
./s2html input.c output.html
```

On Windows:

```powershell
.\s2html.exe input.c output.html
```

## 🌐 View the Generated HTML

Open the generated `.html` file in your web browser.

**Important:** Keep `styles.css` in the same directory as the generated HTML file so the syntax highlighting stylesheet can load correctly.

## 🎨 Syntax Highlighting

The converter styles common source-code elements, including:

- Keywords
- Identifiers
- Numbers
- String and character literals
- Comments
- Preprocessor directives
- Operators
- Special symbols and punctuation

## 📚 What I Learned

- Reading and processing source files in C.
- Generating HTML output programmatically.
- Applying CSS styles to generated content.
- Escaping special HTML characters to preserve source text safely.
- Designing a modular C application with separate components.

## ⚠️ Scope and Limitations

This is a learning project, not a complete C compiler or fully standard-conforming lexer. It provides practical syntax coloring and source preservation, but does not resolve typedef names, implement every C numeric-literal rule, or fully model preprocessor line continuations.

## 👨‍💻 Author

**Praneeth Reedy Nalamalapu**
