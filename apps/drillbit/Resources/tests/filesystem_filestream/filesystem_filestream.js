describe("Ti.Filesystem FileStream tests",{
	before_all:function()
	{
		// clean up testing folder if needed
		var base = Titanium.Filesystem.getFile(Titanium.Filesystem.getApplicationDataDirectory(), "unittest_filesystem_filestream");
		if(base.exists() && base.isDirectory()) {
			base.deleteDirectory(true);
		}
		else if(base.exists() && base.isFile()) {
			base.deleteFile();
		}
	
		base.createDirectory();
		
		this.base = base;
	},
	
	other_props:function()
	{
		value_of(Titanium.Filesystem.getLineEnding).should_be_function();
		value_of(Titanium.Filesystem.getSeparator).should_be_function();
		value_of(Titanium.Filesystem.MODE_READ).should_not_be_null();
		value_of(Titanium.Filesystem.MODE_WRITE).should_not_be_null();
		value_of(Titanium.Filesystem.MODE_APPEND).should_not_be_null();
				
		value_of(Titanium.Filesystem.getLineEnding()).should_not_be_null();
		value_of(Titanium.Filesystem.getSeparator()).should_not_be_null();
	},
	
	write_read:function()
	{
		var textToWrite = "This is the text to write in the file";
		var fs = Titanium.Filesystem.getFileStream(this.base, "writeTestFile.txt");
		fs.open(Titanium.Filesystem.MODE_WRITE);
		fs.write(textToWrite);
		fs.close();
		
		var f = Titanium.Filesystem.getFile(this.base, "writeTestFile.txt");
		value_of(f).should_not_be_null();
		value_of(f.exists()).should_be_true();
		
		// read back the file contents
		fs.open(Titanium.Filesystem.MODE_READ);
		var textRead = fs.read();
		fs.close();
		value_of(textRead).should_be_object();
		value_of(textToWrite).should_be(textRead);
		
		// test the blob interface
		value_of(textRead.length).should_be_number();
		value_of(textRead.length).should_be(textToWrite.length);
		value_of(textRead.toString()).should_be(textToWrite);
		value_of(textRead.replace("ABC","EDF")).should_be(textToWrite);
		value_of(textRead.charAt(0)).should_be(textToWrite.charAt(0));
		value_of(textRead.substring(0)).should_be(textToWrite.substring(0));
		value_of(textRead.substring(0,2)).should_be(textToWrite.substring(0,2));
		value_of(textRead.toUpperCase()).should_be(textToWrite.toUpperCase());
		value_of(textRead.toLowerCase()).should_be(textToWrite.toLowerCase());
		value_of(textRead.indexOf('i')).should_be(textToWrite.indexOf('i'));
		value_of(textRead.lastIndexOf('l')).should_be(textToWrite.lastIndexOf('l'));
	},
	
	write_new_line:function()
	{
		var filename = "writeAppendTestFile.txt";
		var textToWrite = "Line";
		
		var fs = Titanium.Filesystem.getFileStream(this.base, filename);
		fs.open(Titanium.Filesystem.MODE_WRITE);
		fs.writeLine(textToWrite);
		fs.close();
		
		var fs2 = Titanium.Filesystem.getFileStream(this.base, filename);
		fs2.open(Titanium.Filesystem.MODE_READ);
		var expectedText = fs2.readLine();
		fs2.close();


		value_of(expectedText).should_be(textToWrite);

		var fs3 = Titanium.Filesystem.getFileStream(this.base, filename);
		fs3.open(Titanium.Filesystem.MODE_READ);
		var expectedText2 = fs3.read();
		fs3.close();

		// must exclude the new line
		value_of(expectedText2).should_not_be(textToWrite);
		if (Titanium.platform == 'win32')
		{
			value_of(expectedText2).should_be(textToWrite+'\r\n');
		}
		else
		{
			value_of(expectedText2).should_be(textToWrite+'\n');
		}
	},
	
	write_append_read:function()
	{
		var filename = "writeAppendTestFile.txt";
		var textToWrite = "This is the text to write in the file.";
		var textToAppend = "This is the text to append.";
		
		var fs = Titanium.Filesystem.getFileStream(this.base, filename);
		fs.open(Titanium.Filesystem.MODE_WRITE);
		fs.write(textToWrite);
		fs.close();
		
		fs = Titanium.Filesystem.getFileStream(this.base, filename);
		fs.open(Titanium.Filesystem.MODE_APPEND);
		fs.write(textToAppend);
		fs.close();
		
		var f = Titanium.Filesystem.getFile(this.base, filename);
		value_of(f).should_not_be_null();
		value_of(f.exists()).should_be_true();
		
		// read back the file contents
		fs.open(Titanium.Filesystem.MODE_READ);
		var textRead = fs.read();
		fs.close();
		value_of(textToWrite+textToAppend).should_be(textRead);	
	}

});


