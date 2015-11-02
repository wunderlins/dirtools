/**
 * Demonstrates usage of a border layout.
 */	
Ext.define('console4.view.layout.Border', {
	extend: 'Ext.panel.Panel',
	xtype: 'layout-border',
	requires: [
		'Ext.layout.container.Border'
	],
	layout: 'border',
	width: 'auto',
	height: 800,
	flex: 1,

	bodyBorder: false,
	
	defaults: {
		collapsible: true,
		split: true,
		bodyPadding: 10
	},

	items: [
		{
			title: '',
			region: 'south',
			height: 100,
			minHeight: 75,
			maxHeight: 150,
			html: '<p>Footer content</p>'
		},
		{
			title: 'Navigation',
			region:'west',
			floatable: false,
			margin: '5 0 0 0',
			width: 125,
			minWidth: 100,
			maxWidth: 250,
			html: '<p>Secondary content like navigation links could go here</p>'
		},
		{
			title: 'Main Content',
			collapsible: false,
			region: 'center',
			margin: '5 0 0 0',
			html: '<h2>Main Page</h2><p>This is where the main content would go</p>'
		}
	]

});

Ext.application({
	name   : 'console4',

	launch : function() {
		var borderLayout = Ext.create('console4.view.layout.Border', {
			renderTo: Ext.getBody(),
		});
		
		new Ext.Viewport({
			title: '',
			layout: 'fit',
			items: [borderLayout]
		});
		
	}
});
