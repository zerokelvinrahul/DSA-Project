# DSA-Project
## README.md

### Similarity Comparison Program Using Suffix Trees and Levenshtein Distance

This program calculates the similarity between two text documents using two methods: the **Longest Common Substring (LCS)** algorithm based on suffix trees, and the **Levenshtein Distance** algorithm. It combines these metrics to produce a comprehensive similarity score.

### Features
- **Levenshtein Distance**: Measures the number of edits (insertions, deletions, substitutions) needed to transform one string into another.
- **Longest Common Substring**: Uses suffix trees to find the longest substring common to both documents.
- **Text Filtering**: Filters out non-alphabetic characters and retains only letters and spaces in text files.
- **File and Text Input Options**: Allows input as either text files or direct text input.

### Usage

1. Compile the code and run the executable.
2. Select input mode:
   - Enter **"f"** for file input.
   - Enter **"t"** for direct text input.
3. Follow prompts to provide file paths or input text.
4. Set the **alpha value** (between 0 and 1) to adjust the weighting:
   - **Alpha = 1**: prioritize Longest Common Substring.
   - **Alpha = 0**: prioritize Levenshtein Distance.
   - Intermediate values combine both metrics proportionally.

### Key Functions

- **`levenshteinDistance`**: Computes the edit distance between two strings.
- **`buildSuffixTree`**: Builds a suffix tree for the combined input text.
- **`getLongestCommonSubstring`**: Retrieves the longest common substring.
- **`text_filter`**: Filters the input text file to keep only letters and spaces.
- **`similarityScore`**: Combines both metrics to produce a similarity score.

### Notes
- The program dynamically manages memory for the suffix tree. To prevent memory leaks, all nodes are freed after use.
- Ensure input files exist and are accessible; otherwise, an error will be displayed.
