function T = vpTree(X)
% function T = vpTree(X)
% computes the vantage point tree structure with
% T.vp : the vantage point
% T.md : the median distance of the vantage point to the other points
% T.idx : the index of the vantage point in the original set
% T.inner and T.outer : vantage-point subtrees
% of a set of n points in d dimensions with coordinates X(1:n,1:d)
%

    T = vpt(X, 1:size(X,1));

end