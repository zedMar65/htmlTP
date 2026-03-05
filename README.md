# HTMLTP - html template compilation server

- Note: this project is very much in pre-pre-pre production so any usage is not recomended and any support is very much required TwT
- Rest of readme will be only todos and implemented tasks, untill the project is ready for deployment

--

## Support: view TODO: comments, would be much apreciated

### Template:

- Imported with name
- Vars can be replaced by string values or templates
- Templates are loaded only upon request
- Dynamic template requesting is made by replacing templates with requests for templates to the backend
- Page only updates templates
- Server only serves templates
- Specific templates on server can be either always loaded to ram, loaded uponon each request or have a timeout
- Template modification happens AFTER copying the template from ram into unique identifier
- Server isn't actually a web server but only provides complete html pages
- Templates should have optioning by js on/off
- Templates should work as websites themselves
- Templates should only be of a SINGLE file type and it should be identified in the template
- Templates can be rendered 1 step OR untill no render is possible
- Template renders can be seen as another template
- Template render: compilation result
- Template template: compilation instructions
