class Solution {
private:
    static int max(int a, int b) {
        return a > b ? a : b;
    }

    static int longestCommonSubsequence(const string &text1, const string &text2) {
        auto sz1 = text1.size(), sz2 = text2.size();
        vector<vector<int>> dp(sz1 + 1, vector<int>(sz2 + 1, 0));
        for (int i = 1; i <= sz1; ++i) {
            for (int j = 1; j <= sz2; ++j) {
                if (text1[i - 1] == text2[j - 1])
                    dp[i][j] = dp[i - 1][j - 1] + 1;
                else
                    dp[i][j] = max(dp[i][j - 1], dp[i - 1][j]);
            }
        }
        return dp[sz1][sz2];
    }

public:
    int minDistance(const string &word1, const string &word2) {
        auto commonLen = longestCommonSubsequence(word1, word2);
        return word1.size() + word2.size() - 2 * commonLen;
    }
};
