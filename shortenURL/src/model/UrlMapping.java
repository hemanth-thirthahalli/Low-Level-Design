package model;

public class UrlMapping {
	
	private final String shortUrl;
	private final String longUrl;
	
	public UrlMapping (String shortUrl, String longUrl) {
		this.shortUrl = shortUrl;
		this.longUrl = longUrl;
	}
	
	public String getShortUrl() {
		return shortUrl;
	}
	
	public String getLongUrl() {
		return longUrl;
	}

}     
