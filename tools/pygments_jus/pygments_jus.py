"""JUS programlama dili için Pygments sözcük çözümleyicisi.

Belge sitesinde ve Pygments kullanan diğer araçlarda ```jus kod bloklarının
renklendirilmesini sağlar. Kurulum: pip install ./tools/pygments_jus
"""
import re

from pygments.lexer import RegexLexer, words
from pygments.token import Comment, Keyword, Name, Number, Operator, Punctuation, String, Text

__all__ = ["JusLexer"]

# Adlar Türkçe harf içerebildiği için kelime sınırı \b ile değil, çevresine bakılarak belirlenir.
WORD_START = r"(?<![\w])"
WORD_END = r"(?![\w])"
IDENTIFIER = r"[^\W\d]\w*"

CONTROL = (
    "eğer", "değilse", "iken", "her", "dön", "devam", "kır", "geç",
    "dene", "yakala", "fırlat", "kullan", "olarak",
)
DECLARATIONS = ("değişken", "fonksiyon", "sınıf")
WORD_OPERATORS = ("ve", "veya", "değil", "içinde")
CONSTANTS = ("doğru", "yanlış", "boş")
PSEUDO = ("bu", "üst")
BUILTINS = (
    "yaz", "oku", "metin", "sayı", "tür", "uzunluk", "örneği_mi", "doğrula", "eşit_olmalı",
    "karekök", "mutlak", "taban", "tavan", "yuvarla", "biçimle", "aralık", "saat",
    "ekle", "araya_ekle", "çıkar", "sil", "sırala", "eşle", "süz", "ters", "bul",
    "anahtarlar", "değerler", "al", "büyük_harf", "küçük_harf", "kırp", "değiştir",
    "böl", "birleştir", "tekrarla", "sola_doldur", "sağa_doldur", "başlar_mı", "biter_mi",
)


def keyword_rule(names, token):
    return (words(names, prefix=WORD_START, suffix=WORD_END), token)


class JusLexer(RegexLexer):
    name = "JUS"
    aliases = ["jus"]
    filenames = ["*.jus"]
    flags = re.UNICODE | re.MULTILINE

    tokens = {
        "root": [
            (r"#.*?$", Comment.Single),
            # Biçimli metin (f"...") ve sıradan metin.
            (r'f"', String.Interpol, "fstring"),
            (r'"', String, "string"),
            (r"\d+(?:\.\d+)?", Number),
            keyword_rule(DECLARATIONS, Keyword.Declaration),
            keyword_rule(CONTROL, Keyword),
            keyword_rule(WORD_OPERATORS, Operator.Word),
            keyword_rule(CONSTANTS, Keyword.Constant),
            keyword_rule(PSEUDO, Name.Builtin.Pseudo),
            (words(BUILTINS, prefix=r"(?<![\w.])", suffix=r"(?=\s*\()"), Name.Builtin),
            (IDENTIFIER + r"(?=\s*\()", Name.Function),
            (IDENTIFIER, Name),
            (r"==|!=|<=|>=|<<|>>|\+=|-=|\*=|/=|[+\-*/%<>=&|^~]", Operator),
            (r"[()\[\]{},.:]", Punctuation),
            (r"\s+", Text),
            (r".", Text),
        ],
        "fstring": [
            (r"\{\{|\}\}", String.Escape),
            (r"\{[^{}\"\n]*\}", String.Interpol),
            (r'\\[ntr"\\]', String.Escape),
            (r'"', String.Interpol, "#pop"),
            (r'[^"\\\n{}]+', String),
            (r"\\.", String),
            (r"\n", Text, "#pop"),
        ],
        "string": [
            (r'\\[ntr"\\]', String.Escape),
            (r'"', String, "#pop"),
            (r'[^"\\\n]+', String),
            (r"\\.", String),
            (r"\n", Text, "#pop"),
        ],
    }
