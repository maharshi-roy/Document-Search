import React, { Component, Fragment } from 'react';
import { Button, TextField, Layout, Logo, AutoComplete, Snackbar, Collation } from './components';
import { withStyles, Paper, Radio, InputLabel, FormControlLabel, FormControl } from '@material-ui/core';

import AdobeLogo from '../../images/AdobeLogo.png';
import { readData, writeData } from './utils';

const { ipcRenderer } = window.require('electron');

const styles = theme => ({
    root: {
        display: 'flex',
        flex: 10,
        margin: 'auto',
        width: '50%',
    },
    logo: {
        padding: 10,
        flex: 1,
    },
    body: {
        flex: 3,
        margin: 'auto',

        '& > *' : {
            padding: 10
        }
    },
    input: {
        margin: 'auto',
        display: 'flex',
        justifyContent: 'flex-end',
        '& > *': {
            padding: 5,
        }
    },
});

class App extends Component {
    constructor(props) {
        super(props);

        this.state = {
            appData: readData(),
            directory: { value: '', notIndexed: true, completed: false, loading: false },
            query: {
                value: '',
                completed: false,
                loading: false,
                result: null
            },
            collation: { input: 'H', output: 'H' },
            suggestions: { halfWord: '', list: [] },
            error: { message: '', completed: false }
        };

        ipcRenderer.on('results', (event, message) => {
            const instruction = message.substr(0, 3);
            var data = message.substr(4);

            switch(instruction) {
                case "DIC":
                    writeData({ ...this.state.appData, directory: data });
                    this.setState({ directory: { 
                        ...this.state.directory, 
                        notIndexed: false,
                        completed: true,
                        loading: false,
                    }});
                    break;
                case "KIC":
                    const collation = data[0];
                    data = data.substr(2);
                    this.setState({ 
                        query: {
                            ...this.state.query,
                            result: this.processResults(data),
                            completed: true,
                            loading: false,
                        },
                        collation: {
                            ...this.state.collation,
                            output: collation
                        }
                    });
                    break;
                case "KAC":
                    const temp = data.split(";");
                    this.setState({ suggestions: {
                        halfWord: (temp==undefined || temp.length==0) ? '' : temp[0],
                        list: (temp[0].length==0) ? [] : temp[1].split(':')
                    }});
                    break;
                case "ERR":
                    this.setState({ 
                        error: {
                            message: data,
                            completed: true
                        },
                        directory: {
                            ...this.state.directory,
                            loading: false
                        },
                        query: {
                            ...this.state.query,
                            loading: false
                        }
                    });
                    break;
            }
        });

        this.directoryRef = React.createRef();
    }

    componentDidMount() {
        if (this.state.appData.directory)
            this.setState({
                directory: {
                    value: this.state.appData.directory,
                    notIndexed: false
                }
            });
        
        //this.directoryRef.current.type = "file";
        this.directoryRef.current.directory = true;
        this.directoryRef.current.webkitdirectory = true;
    }

    processResults = (data) => {
        var temp = data.split('$');
        temp.shift();

        var result = {};

        temp.map(each => {
            var temp2 = each.split('@');
            var key = temp2[0];

            result[key] = [];

            for (var i=1; i<temp2.length; i++) {
                var temp3 = temp2[i].split(';');
                result[key].push(temp3);
            }
        })

        return result;
    }

    changeQuery = (value) => {
        ipcRenderer.send('data', 'KA|' + value + '\n');
        this.setState({ query: { ...this.state.query, value: value } });
    }

    reactToCPPviaNode = (data) => ipcRenderer.send('data', data)

    chooseSuggestion = (suggestion) => {
        this.setState({
            suggestions: {
                halfWord: '',
                list: []
            },
            query: {
                ...this.state.query,
                value: this.state.query.value + suggestion,
            }
        })
    }

    toggleCollation = () => this.setState({ 
        collation: {
            ...this.state.collation,
            input: ((this.state.collation.input == 'H') ? 'V' : 'H') }
        })

    indexDirectory = () => {
        const { directory } = this.state;
        
        this.setState({ directory: { 
            ...directory, 
            notIndexed: true, 
            loading: true,
        }});

        this.reactToCPPviaNode('DI|' + directory.value + '\n');
    }

    search = () => {
        const { query, collation } = this.state;
        this.setState({
            query: {
                ...query,
                loading: true,
            }
        });

        this.reactToCPPviaNode('KI|' + collation.input + '|' + query.value + '\n');
    }

    render() {
        const { directory, query, suggestions, collation, error } = this.state;
        const { classes } = this.props;

        return (
            <Layout indexing={(directory.loading || query.loading)}>
                <Paper className={classes.root}>
                    <div className={classes.logo}>
                        <Logo Image={AdobeLogo} />
                    </div>
                    <div className={classes.body}>
                        <div className={classes.input}>
                            <TextField 
                                label={'Change Directory'}
                                value={directory.value}
                                onClick={() => this.directoryRef.current.click()}
                            />
                            <input
                                type="file"
                                style={{ display: 'none' }}
                                ref={this.directoryRef}
                                onChange={(event) => this.setState({
                                    directory: {
                                        ...this.state.directory,
                                        value: event.target.files[0].path
                                    }
                                })}
                            />
                            <Button 
                                label={'Update'}
                                callFunc={this.indexDirectory}
                            />
                        </div>
                        {!directory.notIndexed &&
                            <Fragment>
                                <div className={classes.input}>
                                    <AutoComplete
                                        label={'Keyword'}
                                        value={query.value}
                                        handleText={this.changeQuery}
                                        suggestions={suggestions}
                                        chooseSuggestion={this.chooseSuggestion}
                                        onKeyPress={(event) => {
                                            if (event.key == 'Enter')
                                                this.search();
                                        }}
                                    />
                                    <Button 
                                        label={'Search'}
                                        callFunc={this.search}
                                    />
                                </div> 
                            <div className={classes.input} style={{ justifyContent: 'center' }}>
                                <FormControlLabel
                                    value="end"
                                    control={<Radio
                                                checked={(collation.input == 'H')}
                                                onChange={this.toggleCollation}
                                            />}
                                    label="Horizontal"
                                    labelPlacement="end"
                                />
                                <FormControlLabel
                                    value="end"
                                    control={<Radio
                                                checked={(collation.input == 'V')}
                                                onChange={this.toggleCollation}
                                            />}
                                    label="Vertical"
                                    labelPlacement="end"
                                />
                            </div>
                        </Fragment>
                        }                  
                    </div>
                </Paper>
                <Snackbar 
                    open={directory.completed} 
                    handleClose={() => this.setState({
                        directory: {
                            ...directory,
                            completed: false
                        }
                    })}
                    type={{
                        variant: 'success',
                        message: 'Directory Indexed'
                    }}
                />
                <Snackbar 
                    open={query.completed} 
                    handleClose={() => this.setState({
                        query: {
                            ...query,
                            completed: false
                        }
                    })}
                    type={{
                        variant: 'success',
                        message: 'Search Results'
                    }}
                />
                <Snackbar 
                    open={error.completed} 
                    handleClose={() => this.setState({
                        error: {
                            ...error,
                            completed: false
                        }
                    })}
                    type={{
                        variant: 'error',
                        message: error.message
                    }}
                />
                {query.result != null && 
                    ((collation.output == 'V') ?
                        <Collation data={query.result} columns={["Keyword", "Count"]} />
                        :
                        <Collation data={query.result} columns={["File", "Count"]} />
                    )
                }
            </Layout>
        );
    }
}

export default withStyles(styles)(App);