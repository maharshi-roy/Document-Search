import React, { Component, Fragment } from 'react';
import { withStyles, Paper, MenuItem } from '@material-ui/core';
import TextField from './TextField';

const styles = theme => ({
    root: {
        position: 'relative',
        flex: 3,
        display: 'flex'
    },
    suggestions: {
        position: 'absolute',
        top: 55,
        width: '100%',
        maxHeight: 200,
        overflow: 'auto',
        zIndex: 1
    }
});

class AutoComplete extends Component {
    constructor(props) {
        super(props);
    }

    render() {
        const { label, value, handleText, classes, suggestions, chooseSuggestion, ...restProps } = this.props;
        console.log(suggestions);

        return (
            <div className={classes.root}>
                <TextField
                    label={label} 
                    value={value}
                    handleText={handleText}
                    {...restProps}
                />
                {suggestions.list.length!=0 &&
                    <Paper
                        className={classes.suggestions}
                    >
                        {suggestions.list.map((each,index) => {
                            return (
                                        (each.length!=0) &&
                                        <MenuItem key={index} onClick={() => {
                                            chooseSuggestion(each);
                                            this.setState({ anchorEl: null })
                                        }}>{suggestions.halfWord + each}
                                        </MenuItem>
                                    );
                            }
                        )}
                    </Paper>
                }
            </div>
        )
    }
}

export default withStyles(styles)(AutoComplete);