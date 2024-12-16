namespace DotnetEncodingTesting
{
    internal class Program
    {
        static void Main(string[] args)
        {
            // Single unicode character.
            // https://www.fileformat.info/info/unicode/char/045e/index.htm
            var s1 = "ў";

            // Two unicode characters combined.
            // https://www.fileformat.info/info/unicode/char/0443/index.htm
            // https://www.fileformat.info/info/unicode/char/0306/index.htm
            var s2 = "ў";

            Console.WriteLine($"String 1: {s1} - length: {s1.Length}");
            Console.WriteLine($"String 2: {s2} - length: {s2.Length}");

            // Resolves to .Equals(x, y) for strings.
            Console.WriteLine($"String equality: {s1 == s2}");

            // Returns early as it sees that the lengths of the strings are not the same.
            Console.WriteLine($"String equals default: {s1.Equals(s2)}");

            // Ordinal comparison is essentially a binary comparison, and therefore returns false.
            Console.WriteLine($"String equals ordinal: {s1.Equals(s2, StringComparison.Ordinal)}");

            // These options assert that the strings should be compared linquistically. That is, compare them to see if they are
            // semantically the same, rather than binary equals. Internally this path uses an ICU comparison method via (on Windows)
            // the CompareString Win32 function. This appears to do some normalize first, before comparison, and therefore returns true.
            // See also: https://learn.microsoft.com/en-us/windows/win32/api/stringapiset/nf-stringapiset-comparestringw
            // And: https://icu4c-demos.unicode.org/icu-bin/scompare
            Console.WriteLine($"String equals current culture: {s1.Equals(s2, StringComparison.CurrentCulture)}");
            Console.WriteLine($"String equals invariant: {s1.Equals(s2, StringComparison.InvariantCulture)}");
        }
    }
}
