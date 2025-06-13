package utils;

public class Base62Encoder {
	
/*	Base62 means using 62 characters to represent numbers:

		26 lowercase letters → a to z (0–25)

		26 uppercase letters → A to Z (26–51)

		10 digits → 0 to 9 (52–61)  */
	
	private static final String CHAR_MAP = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";	
	
	public static String encode(long number) {
		StringBuilder sb = new StringBuilder();
		while(number > 0) {
			sb.append(CHAR_MAP.charAt((int)(number % 62)));
			number /= 62;
		}
		
		return sb.reverse().toString();
	}

}


		/* Iteration 1:
		125 % 62 = 1 → CHAR_MAP[1] = b
		
		Add 'b' to sb
		
		number = 125 / 62 = 2
		
		Iteration 2:
		2 % 62 = 2 → CHAR_MAP[2] = c
		
		Add 'c' to sb
		
		number = 2 / 62 = 0 → Done
		
		Now, sb = "bc" → reverse it → cb
		
		Final Result: "cb"  */



