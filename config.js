function loadAnnot(fdfPath) {
  try { 
    this.importAnFDF(fdfPath);		 
  } catch (e) {
    console.println("error retrieving annots from file : " + e);
  }
}
